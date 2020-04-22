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
	LoRa::LoRa(SPI_HandleTypeDef* hspi, GPIO_TypeDef* gpio0, uint16_t dio0, GPIO_TypeDef* gpio1, uint16_t dio1,
			   GPIO_TypeDef* gpio2, uint16_t dio2, GPIO_TypeDef* gpio3, uint16_t dio3, GPIO_TypeDef* gpiorx, uint16_t rxenpin,
			   GPIO_TypeDef* gpiotx, uint16_t txenpin, GPIO_TypeDef* gpiocs, uint16_t cspin, GPIO_TypeDef* gpiorst, uint16_t rstpin) : SPI_Device(hspi), Module("LoRaModem 0.1")
	{
		//ctor
		_gpio0 = gpio0;
		_dio0 = dio0;
		_gpio1 = gpio1;
		_dio1 = dio1;
		_gpio2 = gpio2;
		_dio2 = dio2;
		_gpio3 = gpio3;
		_dio3 = dio3;

		_gpiorx = gpiorx;
		_rxenpin = rxenpin;
		_gpiotx = gpiotx;
		_txenpin = txenpin;
		_gpiocs = gpiocs;
		_cspin = cspin;
		_gpiorst = gpiorst;
		_rstpin = rstpin;

		_bandwidth = 0x07;
		_codingRate = 0x01;
		_implicitHeader = 0x00;
		_spreadingFactor = 0x07;
		_rxPayloadCrc = 0;
		_regSymbTimeout = 0x064;
		_preambLength=0x0008;
		_payloadLength = 0x01;
		_payloadMaxLength = 0xff;
		_freqHoppingPeriod = 0x00;
		_syncWord = 0x12;
		_txPowerReg = 0x4f;
		_ocp = 0x2b;
		_isrFlag = false;

		_tx_idx = 0;
		_tx_size = 0;
		for(int i = 0; i < 129; ++i) _tx_buf[i] = 0;
	}

	int LoRa::init(uint64_t workingFreq)
	{
		if(try_lock() != 0) return EBUSY;

		HAL_GPIO_WritePin(_gpiorst, _rstpin, GPIO_PIN_RESET);
		HAL_Delay(1);
		HAL_GPIO_WritePin(_gpiorst, _rstpin, GPIO_PIN_SET);

		_tx_buf[0] = 0x42;
		HAL_GPIO_WritePin(_gpiocs, _cspin, GPIO_PIN_RESET);
		HAL_SPI_TransmitReceive(_hspi, _tx_buf, _rx_buf, 2, 1000);
		HAL_GPIO_WritePin(_gpiocs, _cspin, GPIO_PIN_SET);
		_chipVersion = _rx_buf[1];

		setOpMode(OpMode::Sleep);

		uint64_t frf = (workingFreq << 19)/32000000;
		_workingFreq = frf;
		_queueCommand(WriteReg | RegFrLsbAddr, frf & 0xff);
		_queueCommand(WriteReg | RegFrMidAddr, (frf >> 8) & 0xff);
		_queueCommand(WriteReg | RegFrMsbAddr, (frf >> 16) & 0xff);

		_queueCommand(WriteReg | RegModemConfig3Addr, 0x04);

		setTxPower(4, 1); // 4dbi, boost pin

		setOpMode(OpMode::Standby);

		_isrFlag = true;

		return 0;
	}

	int LoRa::setImplicitHeader(uint8_t ih)
	{
		if(try_lock() != 0) return EBUSY;

		if(ih == 0) _implicitHeader = 0;
		else _implicitHeader = 1;
		_queueCommand(WriteReg | RegModemConfig1Addr, (_bandwidth<<4) | (_codingRate<<1) | _implicitHeader);

		return 0;
	}

	uint8_t LoRa::getImplicitHeader()
	{
		return _implicitHeader;
	}


	int LoRa::setSpreadingFactor(uint8_t sf)
	{
		if(try_lock() != 0) return EBUSY;

		_spreadingFactor = sf;
		_queueCommand(WriteReg | RegModemConfig2Addr, (_spreadingFactor << 4) | (_rxPayloadCrc << 2) | ((_regSymbTimeout >> 8) & 0x3));

		return 0;
	}

	uint8_t LoRa::getSpreadingFactor()
	{
		return _spreadingFactor;
	}


	int LoRa::setPreambleLength(uint16_t pl)
	{
		if(try_lock() != 0) return EBUSY;

		_preambLength = pl;
		_queueCommand(WriteReg | RegPreambleMsbAddr, (_preambLength >> 8) & 0xff);
		_queueCommand(WriteReg | RegPreambleLsbAddr, (_preambLength) & 0xff);

		return 0;
	}

	uint16_t LoRa::getPreambleLength()
	{
		return _preambLength;
	}

	int LoRa::setRegSymbTimeout(uint16_t symbTimeout)
	{
		if(try_lock() != 0) return EBUSY;

		_regSymbTimeout = symbTimeout;
		_queueCommand(WriteReg | RegModemConfig2Addr, (_spreadingFactor << 4) | (_rxPayloadCrc << 2) | ((_regSymbTimeout >> 8) & 0x3));
		_queueCommand(WriteReg | RegSymbTimeoutLsbAddr, _regSymbTimeout & 0xff);

		return 0;
	}

	uint16_t LoRa::getRegSymbTimeout()
	{
		return _regSymbTimeout;
	}


	int LoRa::setBandwidth(uint8_t bw)
	{
		if(try_lock() != 0) return EBUSY;

		_bandwidth = bw;
		_queueCommand(WriteReg | RegModemConfig1Addr, (_bandwidth<<4) | (_codingRate<<1) | _implicitHeader);

		return 0;
	}

	uint8_t LoRa::getBandwidth()
	{
		return _bandwidth;
	}


	int LoRa::setSyncWord(uint8_t sw)
	{
		if(try_lock() != 0) return EBUSY;

		_syncWord = sw;
		_queueCommand(WriteReg | RegSyncWordAddr, _syncWord);

		return 0;
	}

	uint8_t LoRa::getSyncWord()
	{
		return _syncWord;
	}

	int LoRa::getWorkingFrequency()
	{
		return _workingFreq;
	}

	int LoRa::setTxPower(int txPower, uint8_t boostPin)
	{
		if(try_lock() != 0) return EBUSY;

		if(boostPin == 0) //RFO_HF
		{
			if(txPower < 0) txPower = 0;
			else if(txPower > 14) txPower = 14;
			_queueCommand(WriteReg | RegPaConfigAddr, 0x70 | txPower);
			_txPowerReg = txPower;
		}
		else
		{
			if(txPower > 17)
			{
				if(txPower > 20) txPower = 20;
				txPower -= 3; // se hace lo mismo pero cambiando el vapor en regPaDac
				_queueCommand(WriteReg | RegPaDacAddr, 0x87);
				setOCP(140);
			}
			else
			{
				if(txPower < 2) txPower = 2;
				_queueCommand(WriteReg | RegPaDacAddr, 0x84);
				setOCP(100);
			}
			_txPowerReg = 0x80 | (txPower-2);
			_queueCommand(WriteReg | RegPaConfigAddr, _txPowerReg);
		}
		return 0;
	}

	uint8_t LoRa::getTxPower()
	{
		if(_txPowerReg & 0x80)
			return (_txPowerReg & 0x7f)-2;
		else
			return (_txPowerReg & 0x0f);
	}

	uint8_t LoRa::getOutputPin()
	{
		return (_txPowerReg & 0x80) != 0 ? 1 : 0;
	}

	int LoRa::setOCP(uint8_t ocpMA)
	{
		if(try_lock() != 0) return EBUSY;

		uint8_t ocpTrim = 27;

		if(ocpMA <= 120)
			ocpTrim = (ocpMA - 45) / 5;
		else if(ocpMA <= 240)
			ocpTrim = (ocpMA + 30) / 10;

		_ocp = 0x20 | ocpTrim;
		_queueCommand(RegOcpAddr, _ocp);

		return 0;
	}

	uint8_t LoRa::getChipVersion()
	{
		return _chipVersion;
	}

	int LoRa::setOpMode(OpMode opmode)
	{
		if(try_lock() != 0) return EBUSY;

		if(opmode == OpMode::Tx)
		{
			HAL_GPIO_WritePin(_gpiotx, _txenpin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(_gpiorx, _rxenpin, GPIO_PIN_RESET);
		}
		else if(opmode == OpMode::RxContinuous || opmode == OpMode::RxSingle)
		{
			HAL_GPIO_WritePin(_gpiorx, _rxenpin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(_gpiotx, _txenpin, GPIO_PIN_RESET);
		}
		else
		{
			HAL_GPIO_WritePin(_gpiotx, _txenpin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(_gpiorx, _rxenpin, GPIO_PIN_RESET);
		}

		_opmode = opmode;
		_queueCommand(WriteReg | RegOpModeAddr, 0x80 | static_cast<uint8_t>(opmode));

		return 0;
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

		_isrFlag = true;
		return 0;
	}

	int LoRa::receive(uint8_t continuous)
	{
		if(try_lock() != 0) return EBUSY;

		OpMode opm;
		if(continuous) opm = OpMode::RxContinuous;
		else opm = OpMode::RxSingle;

		_queueCommand(WriteReg | RegDioMapping1Addr, 0x00);

		setOpMode(opm);

		return 0;
	}

	int LoRa::available()
	{
		return _rx_size-_rx_idx;
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
		if(nPin == _dio0)
		{
			if(_opmode == OpMode::RxSingle || _opmode == OpMode::RxContinuous)
			{
				//rxDone isr
				_queueCommand(WriteReg | RegIrqFlagsAddr, 0xc8); // limpiar flags
				_queueCommand(1, 0); // poner un 1 en la cola para comenzar el proceso de lectura
			}
			else if(_opmode == OpMode::Tx)
			{
				//txDone isr
				_queueCommand(WriteReg | RegIrqFlagsAddr, 0xc8);
				setOpMode(OpMode::Standby);
			}
		}
		else if(nPin == _dio1)
		{

		}
		else if(nPin == _dio2)
		{

		}
		else if(nPin == _dio3)
		{

		}
	}

	void LoRa::poll()
	{
		if(getUS() - _lastUs < 5) return;

		if(!_isrFlag) return;
		_isrFlag = false;

		if(_queueAvailable())
		{
			_popCommand(); // send through spi
		}
		else
		{
			unlock();
		}
	}

	void LoRa::ISR()
	{
		_isrFlag = true;
		HAL_GPIO_WritePin(_gpiocs, _cspin, GPIO_PIN_SET);
		_lastUs = getUS();
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
	}

	void LoRa::_popCommand() //  se utilizan las direcciones reservadas como instrucciones para la máquina de estados
	{
		HAL_GPIO_WritePin(_gpiocs, _cspin, GPIO_PIN_RESET);
		if(_tx_queue[_tx_idx] == 0) // escribir mensaje y ale
		{
			HAL_GPIO_WritePin(_gpiotx, _txenpin, GPIO_PIN_SET);
			_tx_buf[0] = WriteReg; // | 0
			_tx_idx++;
			startWrite(_tx_buf, _tx_buf_len+1);
		}
		else if(_tx_queue[_tx_idx] == 2) //leer el número de bytes recibidos
		{
			_tx_buf[0] = RegRxNbBytesAddr;
			_tx_queue[_tx_idx] = 2;
			startTxRx(_tx_buf, _rx_buf, 2);
		}
		else if(_tx_queue[_tx_idx] == 3) // configuracion de la pila fifo
		{
			_rx_size = _rx_buf[1];
			_tx_buf[0] = RegFifoAddrPtr;
			_tx_buf[1] = 0;

			_tx_queue[_tx_idx] = 3;
			startWrite(_tx_buf, 2);
		}
		else if(_tx_queue[_tx_idx] == 4) // recibir los datos de la pila
		{
			_tx_buf[0] = 0;
			_rx_idx = 0;
			_tx_idx++;
			startTxRx(_tx_buf, _rx_buf, _rx_size+1);
		}
		else startWrite(reinterpret_cast<uint8_t*>(&(_tx_queue[_tx_idx++])), 2);
	}
}
