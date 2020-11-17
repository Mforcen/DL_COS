#include "LoRa.h"

#define RegFifoAddr				0x00
#define RegOpModeAddr			0x01
#define RegFrMsbAddr			0x06
#define RegFrMidAddr			0x07
#define RegFrLsbAddr			0x08
#define RegPaConfigAddr			0x09
#define RegPaRampAddr			0x0a
#define RegOcpAddr				0x0b
#define RegLnaAddr				0x0c

#define RegFifoAddrPtr			0x0d
#define RegFifoTxBaseAddr		0x0e
#define RegFifoRxBaseAddr		0x0f
#define RegFifoRxCurrentAddr	0x10
#define RegIrqFlagsMaskAddr		0x11
#define RegIrqFlagsAddr			0x12
#define RegRxNbBytesAddr		0x13
#define RegRxHeaderCntValueMsb	0x14
#define RegRxHeaderCntValueLsb	0x15
#define RegRxPacketCntValueMsb	0x16
#define RegRxPacketCntValueLsb	0x17
#define RegModemStatAddr		0x18

#define RegPktSnrValueAddr		0x19
#define RegPktRssiValueAddr		0x1a
#define RegRssiValueAddr		0x1b
#define RegHopChannelAddr		0x1c
#define RegModemConfig1Addr		0x1d
#define RegModemConfig2Addr		0x1e
#define RegSymbTimeoutLsbAddr	0x1f

#define RegPreambleMsbAddr		0x20
#define RegPreambleLsbAddr		0x21
#define RegPayloadLengthAddr	0x22
#define RegMaxPayloadLenAddr	0x23
#define RegHopPeriodAddr		0x24
#define RegFifoRxByteAddr		0x25
#define RegModemConfig3Addr		0x26
#define RegFeiMsbAddr			0x28
#define RegFeiMidAddr			0x29
#define RegFeiLsbAddr			0x2a
#define RegRssiWidebandAddr		0x2c

#define ErrataIfFreq2Addr		0x2f
#define ErrataIfFreq1Addr		0x30

#define RegDetectOptimizeAddr	0x31
#define RegInvertIQAddr			0x33
#define RegHighBWOptimize1Addr	0x36
#define RegDetectionThreshold	0x37
#define RegSyncWordAddr			0x39
#define	RegHighBWOptimize2Addr	0x3a
#define RegInvertIQ2Addr		0x3b

#define RegDioMapping1Addr		0x40
#define RegDioMapping2Addr		0x41
#define RegVersionAddr			0x42
#define RegPllHop				0x44
#define RegPaDacAddr			0x4a

#define WriteReg				0x80

namespace FwLogger
{
	LoRa::LoRa(SPI_HandleTypeDef* hspi, GPIO_TypeDef* gpio0, uint16_t dio0, GPIO_TypeDef* gpio1, uint16_t dio1, GPIO_TypeDef* gpiorx, uint16_t rxenpin,
			   GPIO_TypeDef* gpiotx, uint16_t txenpin, GPIO_TypeDef* gpiocs, uint16_t cspin, GPIO_TypeDef* gpiorst, uint16_t rstpin) :
			   SPI_Device(hspi), Module("LoRaModem", LORA_BINID, 0, 1, 15, m_loraParams)
	{
		//ctor
		m_gpio0 = gpio0;
		m_dio0 = dio0;
		m_gpio1 = gpio1;
		m_dio1 = dio1;

		m_gpiorx = gpiorx;
		m_rxenpin = rxenpin;
		m_gpiotx = gpiotx;
		m_txenpin = txenpin;
		m_gpiocs = gpiocs;
		m_cspin = cspin;
		m_gpiorst = gpiorst;
		m_rstpin = rstpin;
		m_isrFlag = false;

		bandwidth = 0x07;
		codingRate = 0x01;
		implicitHeader = 0x00;
		spreadingFactor = 0x07;
		rxPayloadCrc = 0;
		regSymbTimeout = 0x064;
		preambLength=0x0008;
		m_payloadLength = 0x01;
		payloadMaxLength = 0xff;
		freqHoppingPeriod = 0x00;
		syncWord = 0x12;
		txPowerReg = 0x4f;
		ocp = 0x2b;
		regPaDac = 0x84;

		_tx_idx = 0;
		_tx_size = 0;
		for(int i = 0; i < 129; ++i) _tx_buf[i] = 0;
	}

	int LoRa::init(uint64_t init_freq)
	{
		if(try_lock() != 0) return EBUSY;

		HAL_GPIO_WritePin(m_gpiorst, m_rstpin, GPIO_PIN_RESET);
		HAL_Delay(1);
		HAL_GPIO_WritePin(m_gpiorst, m_rstpin, GPIO_PIN_SET);

		_tx_buf[0] = 0x42;
		HAL_GPIO_WritePin(m_gpiocs, m_cspin, GPIO_PIN_RESET);
		HAL_SPI_TransmitReceive(_hspi, _tx_buf, _rx_buf, 2, 1000);
		HAL_GPIO_WritePin(m_gpiocs, m_cspin, GPIO_PIN_SET);
		chipVersion = _rx_buf[1];

		setOpMode(OpMode::Sleep);

		workingFreq = init_freq;
		uint64_t frf = (static_cast<uint64_t>(workingFreq) << 19)/32000000;
		_queueCommand(WriteReg | RegFrLsbAddr, frf & 0xff);
		_queueCommand(WriteReg | RegFrMidAddr, (frf >> 8) & 0xff);
		_queueCommand(WriteReg | RegFrMsbAddr, (frf >> 16) & 0xff);

		_queueCommand(WriteReg | RegModemConfig3Addr, 0x04);

		_queueCommand(WriteReg | RegLnaAddr, 0x23); // boost LNA

		setTxPower(4, 1); // 4dbi, boost pin

		setOpMode(OpMode::Standby);

		m_isrFlag = true;

		return 0;
	}

	int LoRa::setImplicitHeader(uint8_t ih)
	{
		if(ih == 0) implicitHeader = 0;
		else implicitHeader = 1;
		_queueCommand(WriteReg | RegModemConfig1Addr, (bandwidth<<4) | (codingRate<<1) | implicitHeader);

		return 0;
	}

	uint8_t LoRa::getImplicitHeader()
	{
		return implicitHeader;
	}

	int LoRa::setSpreadingFactor(uint8_t sf)
	{
		if(sf < 6 || sf > 12) return -1;

		spreadingFactor = sf;
		_queueCommand(WriteReg | RegModemConfig2Addr, (spreadingFactor << 4) | (rxPayloadCrc << 2) | ((regSymbTimeout >> 8) & 0x3));

		return 0;
	}

	uint8_t LoRa::getSpreadingFactor()
	{
		return spreadingFactor;
	}


	int LoRa::setPreambleLength(uint16_t pl)
	{
		preambLength = pl;
		_queueCommand(WriteReg | RegPreambleMsbAddr, (preambLength >> 8) & 0xff);
		_queueCommand(WriteReg | RegPreambleLsbAddr, (preambLength) & 0xff);

		return 0;
	}

	uint16_t LoRa::getPreambleLength()
	{
		return preambLength;
	}

	int LoRa::setRegSymbTimeout(uint16_t symbTimeout)
	{
		if(symbTimeout > 0x3ff) return -1;

		regSymbTimeout = symbTimeout;
		_queueCommand(WriteReg | RegModemConfig2Addr, (spreadingFactor << 4) | (rxPayloadCrc << 2) | ((regSymbTimeout >> 8) & 0x3));
		_queueCommand(WriteReg | RegSymbTimeoutLsbAddr, regSymbTimeout & 0xff);

		return 0;
	}

	uint16_t LoRa::getRegSymbTimeout()
	{
		return regSymbTimeout;
	}


	int LoRa::setBandwidth(uint8_t bw)
	{
		if(bw>9) return -1;

		bandwidth = bw;
		_queueCommand(WriteReg | RegModemConfig1Addr, (bandwidth<<4) | (codingRate<<1) | implicitHeader);

		return 0;
	}

	uint8_t LoRa::getBandwidth()
	{
		return bandwidth;
	}


	int LoRa::setSyncWord(uint8_t sw)
	{
		if(sw == 0x34) return -1;
		syncWord = sw;
		_queueCommand(WriteReg | RegSyncWordAddr, syncWord);

		return 0;
	}

	uint8_t LoRa::getSyncWord()
	{
		return syncWord;
	}

	int LoRa::getWorkingFrequency()
	{
		return workingFreq;
	}

	int LoRa::setTxPower(int txPower, uint8_t boostPin)
	{
		if(boostPin == 0) //RFO_HF
		{
			if(txPower < 0) txPower = 0;
			else if(txPower > 14) txPower = 14;
			_queueCommand(WriteReg | RegPaConfigAddr, 0x70 | txPower);
			txPowerReg = txPower;
		}
		else
		{
			if(txPower > 18)
			{
				txPower = 0x7f; // full power
				regPaDac = 0x87;
				_queueCommand(WriteReg | RegPaDacAddr, 0x87);
			}
			else
			{
				if(txPower < 2) txPower = 2;
				txPower -= 2;
				regPaDac = 0x84;
				_queueCommand(WriteReg | RegPaDacAddr, 0x84);
			}
			txPowerReg = 0x80 | txPower;
			_queueCommand(WriteReg | RegPaConfigAddr, txPowerReg);
		}
		return 0;
	}

	uint8_t LoRa::getTxPower()
	{
		if(txPowerReg & 0x80)
			if(regPaDac == 0x87)
			{
				if((txPowerReg & 0x0f) == 0xf) return 20;
				else
					return (txPowerReg & 0x0f) + 2;
			}
			else
			{
				return (txPowerReg & 0x0f) + 2;
			}
		else
			return (txPowerReg & 0x0f); // complex calculations including other registers
	}

	uint8_t LoRa::getOutputPin()
	{
		return (txPowerReg & 0x80) != 0 ? 1 : 0;
	}

	int LoRa::setOCP(uint8_t ocpMA)
	{
		uint8_t ocpTrim = 27;

		if(ocpMA <= 120)
			ocpTrim = (ocpMA - 45) / 5;
		else if(ocpMA <= 240)
			ocpTrim = (ocpMA + 30) / 10;

		ocp = 0x20 | ocpTrim;
		_queueCommand(WriteReg | RegOcpAddr, ocp);

		return 0;
	}

	uint8_t LoRa::getOCP()
	{
		uint8_t ocpMA = 0;
		uint8_t ocpTrim = ocp & 0x1f;

		if(ocpTrim <= 15)
			ocpMA = 45+5*ocpTrim;
		else if(ocpTrim <= 27)
			ocpMA = -30+10*ocpTrim;
		return ocpMA;
	}

	uint8_t LoRa::getChipVersion()
	{
		return chipVersion;
	}

	int LoRa::setOpMode(OpMode opmode)
	{
		if(try_lock() != 0) return EBUSY;

		if(opmode == OpMode::Tx)
		{
			HAL_GPIO_WritePin(m_gpiotx, m_txenpin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(m_gpiorx, m_rxenpin, GPIO_PIN_RESET);
		}
		else if(opmode == OpMode::RxContinuous || opmode == OpMode::RxSingle)
		{
			HAL_GPIO_WritePin(m_gpiorx, m_rxenpin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(m_gpiotx, m_txenpin, GPIO_PIN_RESET);
		}
		else
		{
			HAL_GPIO_WritePin(m_gpiotx, m_txenpin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(m_gpiorx, m_rxenpin, GPIO_PIN_RESET);
		}

		m_opmode = opmode;
		_queueCommand(WriteReg | RegOpModeAddr, 0x80 | static_cast<uint8_t>(opmode));

		return 0;
	}

	int LoRa::getConfigSize()
	{
		return 16;
	}

	int LoRa::getConfig(uint8_t* buf)
	{
		buf[0] = bandwidth;
		buf[1] = codingRate;
		buf[2] = implicitHeader;
		buf[3] = spreadingFactor;
		buf[4] = rxPayloadCrc;
		buf[5] = preambLength & 0xff;
		buf[6] = (preambLength >> 8) & 0xff;
		buf[7] = payloadMaxLength;
		buf[8] = freqHoppingPeriod;
		buf[9] = syncWord;
		buf[10] = getTxPower();
		buf[11] = getOCP();
		buf[12] = chipVersion;
		buf[13] = workingFreq & 0xff;
		buf[14] = (workingFreq >> 8) & 0xff;
		buf[15] = (workingFreq >> 16) & 0xff;
		return getConfigSize();
	}

	int LoRa::setConfig(uint8_t* buf)
	{
		setBandwidth(buf[0]);
		//buf[1] = _codingRate;
		setImplicitHeader(buf[2]);
		setSpreadingFactor(buf[3]);
		//buf[4] = _rxPayloadCrc;
		setPreambleLength(buf[5] | ((buf[6] << 8) & 0xff00));
		//buf[6] = (_preambLength >> 8) & 0xff;
		//buf[7] = _payloadMaxLength;
		//buf[8] = _freqHoppingPeriod;
		setSyncWord(buf[9]);
		//buf[10] = _txPowerReg;
		setOCP(buf[11]);
		//buf[12] = _chipVersion;
		//buf[13] = _workingFreq & 0xff;
		//buf[14] = (_workingFreq >> 8) & 0xff;
		//buf[15] = (_workingFreq >> 16) & 0xff;
		return 0;
	}

	int LoRa::setParam(const char* name, int value)
	{
		if(strcmp(name, "bandwidth") == 0) setBandwidth(value);
		//else if(strcmp(name, "codingRate") == 0) setCodingRate(value);
		else if(strcmp(name, "implicitHeader") == 0) setImplicitHeader(value);
		else if(strcmp(name, "spreadingFactor") == 0) setSpreadingFactor(value);
		//else if(strcmp(name, "rxPayloadCrc") == 0) setRxPayloadCrc(value);
		else if(strcmp(name, "preambLength") == 0) setPreambleLength(value);
		//else if(strcmp(name, "payloadMaxLen") == 0) setPayloadMaxLength(value);
		//else if(strcmp(name, "freqHoppingPeriod") == 0) setFreqHoppingPeriod(value);
		else if(strcmp(name, "syncWord") == 0) setSyncWord(value);
		else if(strcmp(name, "txPower") == 0) setTxPower(value, 1);
		else if(strcmp(name, "ocp") == 0) setOCP(value);
		//else if(strcmp(name, "workingFreq") == 0) setWorkingFreq(value);
		return 0;
	}

	int LoRa::getParam(const char* name, int* dst)
	{
		RetModuleParam(name, bandwidth, dst);
		RetModuleParam(name, codingRate, dst);
		RetModuleParam(name, implicitHeader, dst);
		RetModuleParam(name, spreadingFactor, dst);
		RetModuleParam(name, rxPayloadCrc, dst);
		RetModuleParam(name, preambLength, dst);
		RetModuleParam(name, payloadMaxLength, dst);
		RetModuleParam(name, freqHoppingPeriod, dst);
		RetModuleParam(name, syncWord, dst);
		RetModuleParam(name, txPowerReg, dst);
		RetModuleParam(name, ocp, dst);
		RetModuleParam(name, workingFreq, dst);
		return 0;
	}

	int LoRa::getStatus(uint8_t* buf)
	{
		buf[0] = m_PcktRSSI;
		buf[1] = m_recvFlags;
		buf[2] = m_pendingMsg ? _rx_size-_rx_idx : 0; //TODO known bug when rxsize = 0x100 and rxidx = 0
		return 3;
	}

	int LoRa::bin_eval(uint8_t* buf, int buflen, uint8_t* outbuf)
	{
		if(buf[0] == 'c') // config
		{
			outbuf[0] = 'c';
			if(buf[1] == 'g') // get
			{
				outbuf[1] = 'g';
				getConfig(outbuf+2);
				return 2+getConfigSize();
			}
			else if(buf[1] == 's') // set
			{
				outbuf[1] = 's';
				return 2;
			}
			else if(buf[1] == 'c') // chip
			{
				outbuf[1] = 'c';
				outbuf[2] = chipVersion;
				return 3;
			}
			else if(buf[1] == 'b') // bandwidth
			{
				outbuf[1] = 'b';
				if(buf[2] == 'g')
				{
					outbuf[2] = 'g';
					outbuf[3] = bandwidth;
					return 4;
				}
				else if(buf[2] == 's')
				{
					setBandwidth(buf[3]);
					outbuf[2] = 's';
					return 3;
				}
			}
			else if(buf[1] == 's') // spreading factor
			{
				outbuf[1] = 's';
				if(buf[2] == 'g')
				{
					outbuf[2] = 'g';
					outbuf[3] = spreadingFactor;
					return 4;
				}
				else if(buf[2] == 's')
				{
					setSpreadingFactor(buf[3]);
					outbuf[2] = 's';
					return 3;
				}
			}
			else if(buf[1] == 'w') // sync word
			{
				outbuf[1] = 'w';
				if(buf[2] == 'g')
				{
					outbuf[2] = 'g';
					outbuf[3] = syncWord;
					return 4;
				}
				else if(buf[2] == 's')
				{
					setSyncWord(buf[3]);
					outbuf[2] = 's';
					return 3;
				}
			}
			else if(buf[1] == 'p') // preamble length
			{
				outbuf[1] = 'p';
				if(buf[2] == 'g')
				{
					outbuf[2] = 'g';
					outbuf[3] = preambLength & 0xff;
					outbuf[4] = (preambLength >> 8) & 0xff;
					return 5;
				}
				else if(buf[2] == 's')
				{
					setPreambleLength(buf[3] & (buf[4] << 8));
					outbuf[2] = 's';
					return 3;
				}
			}
			else if(buf[1] == 't') // symb timeout
			{
				outbuf[1] = 't';
				if(buf[2] == 'g')
				{
					outbuf[2] = 'g';
					outbuf[3] = regSymbTimeout & 0xff;
					outbuf[4] = (regSymbTimeout >> 8) & 0xff;
					return 5;
				}
				else if(buf[2] == 's')
				{
					setRegSymbTimeout(buf[3] & (buf[4] << 8));
					outbuf[2] = 's';
					return 3;
				}
			}
			else if(buf[1] == 'c') // coding rate
			{
				outbuf[1] = 'c';
				if(buf[2] == 'g')
				{
					outbuf[2] = 'g';
					outbuf[3] = codingRate;
					return 4;
				}
				else if(buf[2] == 's')
				{
					// TODO create set coding rate
					outbuf[2] = 's';
					return 3;
				}
			}
			else if(buf[1] == 'C') // crc
			{
				outbuf[1] = 'C';
				if(buf[2] == 'g')
				{
					outbuf[2] = 'g';
					outbuf[3] = rxPayloadCrc;
					return 4;
				}
				else if(buf[2] == 's')
				{
					//TODO set crc
					outbuf[2] = 's';
					return 3;
				}
			}
			else if(buf[1] == 'l') // max-length
			{
				outbuf[1] = 'l';
				if(buf[2] == 'g')
				{
					outbuf[2] = 'g';
					outbuf[3] = payloadMaxLength;
					return 4;
				}
				else if(buf[2] == 's')
				{
					//TODO set maxlength
					outbuf[2] = 's';
					return 3;
				}
			}
			else if(buf[1] == 'h') // freq hoping
			{
				outbuf[1] = 'h';
				if(buf[2] == 'g')
				{
					outbuf[2] = 'g';
					outbuf[3] = freqHoppingPeriod;
					return 4;
				}
				else if(buf[2] == 's')
				{
					outbuf[2] = 's';
					return 3;
				}
			}
			else if(buf[1] == 'p') // power
			{
				outbuf[1] = 'p';
				if(buf[2] == 'g')
				{
					outbuf[2] = 'g';
					outbuf[3] = getTxPower();
					return 4;
				}
				else if(buf[2] == 's')
				{
					setTxPower(buf[3], 1);
					outbuf[2] = 's';
					return 3;
				}
			}
			else if(buf[1] == 'o') // ocp
			{
				outbuf[1] = 'o';
				if(buf[2] == 'g')
				{
					outbuf[2] = 'g';
					outbuf[3] = getOCP();
					return 4;
				}
				else if(buf[2] == 's')
				{
					setOCP(buf[3]);
					outbuf[2] = 's';
					return 3;
				}
			}

		}
		else if(buf[0] == 's') // status
		{
			outbuf[0] = 's';
			return getStatus(outbuf+1) + 1;
		}
		else if(buf[0] == 'm') // reads last message in memory
		{
			outbuf[0] = 'm';
			outbuf[1] = _rx_size;
			for(int i = 0; i < _rx_size; ++i) outbuf[2+i] = _rx_buf[i+1];
			return _rx_size + 2;
		}
		return 0;
	}

	int LoRa::getPacketRSSI()
	{
		return -157 + m_PcktRSSI;
	}

	int LoRa::getPacketIRQFlags()
	{
		return m_recvFlags;
	}

	int LoRa::getModemStatus()
	{
		if(try_lock() != 0) return -1;
		HAL_GPIO_WritePin(m_gpiocs, m_cspin, GPIO_PIN_RESET);
		uint8_t tx_buf[2] = {RegModemStatAddr, 0};
		uint8_t rx_buf[2] = {0};
		HAL_SPI_TransmitReceive(_hspi, tx_buf, rx_buf, 2, 1000);
		unlock();
		HAL_GPIO_WritePin(m_gpiocs, m_cspin, GPIO_PIN_SET);
		return rx_buf[1];
	}

	int LoRa::getIRQFlags()
	{
		if(try_lock() != 0) return -1;
		HAL_GPIO_WritePin(m_gpiocs, m_cspin, GPIO_PIN_RESET);
		uint8_t tx_buf[2] = {RegIrqFlagsAddr, 0};
		uint8_t rx_buf[2] = {0};
		HAL_SPI_TransmitReceive(_hspi, tx_buf, rx_buf, 2, 1000);
		unlock();
		HAL_GPIO_WritePin(m_gpiocs, m_cspin, GPIO_PIN_SET);
		return rx_buf[1];
	}

	int LoRa::getIRQFlagMasks()
	{
		if(try_lock() != 0) return -1;
		HAL_GPIO_WritePin(m_gpiocs, m_cspin, GPIO_PIN_RESET);
		uint8_t tx_buf[2] = {RegIrqFlagsMaskAddr, 0};
		uint8_t rx_buf[2] = {0};
		HAL_SPI_TransmitReceive(_hspi, tx_buf, rx_buf, 2, 1000);
		unlock();
		HAL_GPIO_WritePin(m_gpiocs, m_cspin, GPIO_PIN_SET);
		return rx_buf[1];
	}

	int LoRa::getRSSI()
	{
		if(try_lock() != 0) return 100;
		HAL_GPIO_WritePin(m_gpiocs, m_cspin, GPIO_PIN_RESET);
		uint8_t tx_buf[2] = {RegRssiValueAddr, 0};
		uint8_t rx_buf[2] = {0};
		HAL_SPI_TransmitReceive(_hspi, tx_buf, rx_buf, 2, 1000);
		unlock();
		HAL_GPIO_WritePin(m_gpiocs, m_cspin, GPIO_PIN_SET);
		return rx_buf[1]-157;
	}

	int LoRa::getRxNbBytes()
	{
		if(try_lock() != 0) return -1;
		HAL_GPIO_WritePin(m_gpiocs, m_cspin, GPIO_PIN_RESET);
		uint8_t tx_buf[3] = {RegIrqFlagsMaskAddr, 0};
		uint8_t rx_buf[3] = {0};
		HAL_SPI_TransmitReceive(_hspi, tx_buf, rx_buf, 3, 1000);
		unlock();
		HAL_GPIO_WritePin(m_gpiocs, m_cspin, GPIO_PIN_SET);
		uint16_t retval = rx_buf[1] | ((rx_buf[2] << 8) & 0xff);
		return retval;
	}

	int LoRa::getRxHeaderCnt()
	{
		if(try_lock() != 0) return -1;
		HAL_GPIO_WritePin(m_gpiocs, m_cspin, GPIO_PIN_RESET);
		uint8_t tx_buf[3] = {RegIrqFlagsMaskAddr, 0};
		uint8_t rx_buf[3] = {0};
		HAL_SPI_TransmitReceive(_hspi, tx_buf, rx_buf, 3, 1000);
		unlock();
		HAL_GPIO_WritePin(m_gpiocs, m_cspin, GPIO_PIN_SET);
		uint16_t retval = rx_buf[1] | ((rx_buf[2] << 8) & 0xff);
		return retval;
	}

	int LoRa::getRxPacketCnt()
	{
		if(try_lock() != 0) return -1;
		HAL_GPIO_WritePin(m_gpiocs, m_cspin, GPIO_PIN_RESET);
		uint8_t tx_buf[3] = {RegIrqFlagsMaskAddr, 0};
		uint8_t rx_buf[3] = {0};
		HAL_SPI_TransmitReceive(_hspi, tx_buf, rx_buf, 3, 1000);
		unlock();
		HAL_GPIO_WritePin(m_gpiocs, m_cspin, GPIO_PIN_SET);
		uint16_t retval = rx_buf[1] | ((rx_buf[2] << 8) & 0xff);
		return retval;
	}

	int LoRa::getFifo(uint8_t addr, int len, uint8_t* buf)
	{
		if(try_lock() != 0) return -1;

		HAL_GPIO_WritePin(m_gpiocs, m_cspin, GPIO_PIN_RESET);
		uint8_t tx_send[2] = {WriteReg | RegFifoAddrPtr, static_cast<uint8_t>(addr & 0xff)};
		HAL_SPI_Transmit(_hspi, tx_send, 2, 1000);
		HAL_GPIO_WritePin(m_gpiocs, m_cspin, GPIO_PIN_SET);

		HAL_Delay(1);

		HAL_GPIO_WritePin(m_gpiocs, m_cspin, GPIO_PIN_RESET);
		tx_send[0] = RegFifoAddr;
		HAL_SPI_Transmit(_hspi, tx_send, 1, 1000);
		HAL_SPI_Receive(_hspi, buf, len, 1000);
		unlock();
		HAL_GPIO_WritePin(m_gpiocs, m_cspin, GPIO_PIN_SET);

		return 0;
	}

	int LoRa::readStatus()
	{
		if(try_lock() != 0) return -1;
		HAL_GPIO_WritePin(m_gpiocs, m_cspin, GPIO_PIN_RESET);
		uint8_t tx_buf[2] = {RegOpModeAddr, 0};
		uint8_t rx_buf[2] = {0};
		HAL_SPI_TransmitReceive(_hspi, tx_buf, rx_buf, 2, 1000);
		unlock();
		HAL_GPIO_WritePin(m_gpiocs, m_cspin, GPIO_PIN_SET);
		return rx_buf[1];
	}

	int LoRa::getRegHopChannel()
	{
		if(try_lock() != 0) return -1;
		HAL_GPIO_WritePin(m_gpiocs, m_cspin, GPIO_PIN_RESET);
		uint8_t tx_buf[2] = { RegHopChannelAddr, 0};
		uint8_t rx_buf[2];
		HAL_SPI_TransmitReceive(_hspi, tx_buf, rx_buf, 2, 1000);
		unlock();
		HAL_GPIO_WritePin(m_gpiocs, m_cspin, GPIO_PIN_SET);
		return rx_buf[1];
	}

	int LoRa::send(uint16_t len, const uint8_t* data)
	{
		if(try_lock() != 0) return EBUSY;

		for(int i = 0; i < len; ++i)
			_tx_buf[i+1] = data[i];

		_tx_buf_len = len;

		_queueCommand(WriteReg | RegDioMapping1Addr, 0x40);
		_queueCommand(WriteReg | RegFifoAddrPtr, 0x80);
		_queueCommand(WriteReg | RegPayloadLengthAddr, _tx_buf_len);
		_queueCommand(0,0); // esto escribe el contenido de tx_buf en la pila

		setOpMode(OpMode::Tx);

		m_isrFlag = true;
		return 0;
	}

	int LoRa::receive(uint8_t continuous)
	{
		if(try_lock() != 0) return EBUSY;

		OpMode opm;
		if(continuous) opm = OpMode::RxContinuous;
		else opm = OpMode::RxSingle;

		_queueCommand(WriteReg | RegDioMapping1Addr, 0x00);
		_queueCommand(WriteReg | RegFifoRxByteAddr, 0);

		setOpMode(opm);

		m_isrFlag = true;

		return 0;
	}

	int LoRa::available()
	{
		if(!m_pendingMsg) return 0;
		int left = _rx_size-_rx_idx;
		if(left == 0) m_pendingMsg = false;
		return left;
	}

	int LoRa::pop()
	{
		if(!available()) return -1;
		uint8_t c = _rx_buf[_rx_idx];
		++_rx_idx;
		return c;
	}

	void LoRa::isrDIO(uint32_t nPin)
	{
		if(nPin == m_dio0)
		{
			if(m_opmode == OpMode::RxSingle || m_opmode == OpMode::RxContinuous)
			{
				//rxDone isr
				//_queueCommand(WriteReg | RegOpModeAddr, 0x80 | static_cast<uint8_t>(OpMode::Standby)); //block rx process
				_queueCommand(5, 0); // poner un 1 en la cola para comenzar el proceso de lectura
				_queueCommand(WriteReg | RegIrqFlagsAddr, 0xf8); // limpiar flags
				_queueCommand(6, 0); // poner un 1 en la cola para comenzar el proceso de lectura
				_queueCommand(7, 0); // obtener direccion de lectura
				_queueCommand(2, 0); // poner un 1 en la cola para comenzar el proceso de lectura
				_queueCommand(3, 0); // poner un 1 en la cola para comenzar el proceso de lectura
				_queueCommand(4, 0); // poner un 1 en la cola para comenzar el proceso de lectura
				//_queueCommand(WriteReg | RegOpModeAddr, 0x80 | static_cast<uint8_t>(_opmode));
				Log::Verbose("Radio message received\n");
			}
			else if(m_opmode == OpMode::Tx)
			{
				//txDone isr
				_queueCommand(WriteReg | RegIrqFlagsAddr, 0xc8);
				setOpMode(OpMode::Standby);
				Log::Verbose("Radio message transmitted\n");
			}
		}
		else if(nPin == m_dio1)
		{

		}
	}

	bool LoRa::loop()
	{
		if(static_cast<uint16_t>(getUS() - m_lastUs) < 5) return true;

		if(!m_isrFlag) return m_waiting;
		m_isrFlag = false;
		m_waiting = false;

		if(_queueAvailable())
		{
			if(try_lock() == 0)
				_popCommand(); // send through spi
			m_waiting = true; // waiting to the isr
		}
		else
		{
			unlock();
		}
		return true;
	}

	void LoRa::ISR()
	{
		m_isrFlag = true;
		if(_tx_queue[_tx_idx] == 2)
		{
			m_payloadLength = _rx_buf[1];
			_rx_size = m_payloadLength+1;
		}
		else if(_tx_queue[_tx_idx] == 4)
		{
			m_pendingMsg = true;
		}
		else if (_tx_queue[_tx_idx] == 5)
		{
			m_recvFlags = _rx_buf[1];
		}
		else if(_tx_queue[_tx_idx] == 6)
		{
			m_PcktRSSI = _rx_buf[1];
		}
		else if(_tx_queue[_tx_idx] == 7)
		{
			m_recvAddr = _rx_buf[1];
		}

		++_tx_idx;
		HAL_GPIO_WritePin(m_gpiocs, m_cspin, GPIO_PIN_SET);
		m_lastUs = getUS();
	}

	int LoRa::_queueAvailable()
	{
		int avail = _tx_size - _tx_idx;
		if(avail == 0)
		{
			if(_tx_size != 0)
			{
				_tx_size = 0;
				_tx_idx = 0;
				for(int i = 0; i < 16; ++i) _tx_buf[i] = 0;
			}
			return 0;
		}
		return avail;
	}

	void LoRa::_queueCommand(uint8_t addr, uint8_t data)
	{
		uint16_t val = addr | ((data << 8) & 0xff00);
		_tx_queue[_tx_size++] = val;
		m_isrFlag = true;
	}

	void LoRa::_popCommand() //  se utilizan las direcciones reservadas como instrucciones para la máquina de estados
	{
		HAL_GPIO_WritePin(m_gpiocs, m_cspin, GPIO_PIN_RESET);
		if(_tx_queue[_tx_idx] == 0) // escribir mensaje y ale
		{
			HAL_GPIO_WritePin(m_gpiotx, m_txenpin, GPIO_PIN_SET);
			_tx_buf[0] = WriteReg; // | 0
			Log::Verbose("Sending radio message\n");
			startWrite(_tx_buf, _tx_buf_len+1);
		}
		else if(_tx_queue[_tx_idx] == 2) //leer el número de bytes recibidos
		{
			_tx_buf[0] = RegRxNbBytesAddr;
			_tx_queue[_tx_idx] = 2;
			Log::Verbose("Recvn message len\n");
			startTxRx(_tx_buf, _rx_buf, 2);
		}
		else if(_tx_queue[_tx_idx] == 3) // configuracion de la pila fifo
		{
			_tx_buf[0] = WriteReg | RegFifoAddrPtr;
			_tx_buf[1] = m_recvAddr;

			Log::Verbose("Reading fifo addr\n");

			startWrite(_tx_buf, 2);
		}
		else if(_tx_queue[_tx_idx] == 4) // recibir los datos de la pila
		{
			_tx_buf[0] = 0;
			_rx_idx = 1;

			Log::Verbose("Recvn fifo data\n");

			startTxRx(_tx_buf, _rx_buf, _rx_size+1);
		}
		else if(_tx_queue[_tx_idx] == 5)
		{
			_tx_buf[0] = RegIrqFlagsAddr;
			Log::Verbose("Recvn flags\n");

			startTxRx(_tx_buf, _rx_buf, 2);
		}
		else if(_tx_queue[_tx_idx] == 6)
		{
			_tx_buf[0] = RegPktRssiValueAddr;
			Log::Verbose("Recvn packet RSSI\n");

			startTxRx(_tx_buf, _rx_buf, 2);

		}
		else if(_tx_queue[_tx_idx] == 7)
		{
			_tx_buf[0] = RegFifoRxCurrentAddr;
			Log::Verbose("Recvn packet fifo addr\n");

			startTxRx(_tx_buf, _rx_buf, 2);
		}
		else startWrite(reinterpret_cast<uint8_t*>(&(_tx_queue[_tx_idx])), 2);
	}
}
