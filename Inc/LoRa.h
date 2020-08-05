#ifndef LORA_H
#define LORA_H

#include "Module.h"
#include "SPI_Device.h"
#include "Log.h"

namespace FwLogger
{
	class LoRa : public SPI_Device, public Module
	{
		public:
			LoRa(SPI_HandleTypeDef* hspi, GPIO_TypeDef* gpio0, uint16_t dio0, GPIO_TypeDef* gpio1, uint16_t dio1, GPIO_TypeDef* gpiorx, uint16_t rxenpin,
				 GPIO_TypeDef* gpiotx, uint16_t txenpin, GPIO_TypeDef* gpiocs, uint16_t cspin, GPIO_TypeDef* gpiorst, uint16_t rstpin);
			int init(uint64_t workingFreq);

			int setImplicitHeader(uint8_t ih);
			uint8_t getImplicitHeader();

			int setSpreadingFactor(uint8_t sf);
			uint8_t getSpreadingFactor();

			int setPreambleLength(uint16_t pl);
			uint16_t getPreambleLength();

			int setRegSymbTimeout(uint16_t symbTimeout);
			uint16_t getRegSymbTimeout();

			int setBandwidth(uint8_t bw);
			uint8_t getBandwidth();

			int setSyncWord(uint8_t sw);
			uint8_t getSyncWord();

			int getWorkingFrequency();

			int setTxPower(int txPower, uint8_t boostPin);
			uint8_t getTxPower();
			uint8_t getOutputPin();

			int setOCP(uint8_t ocp);
			uint8_t getOCP();

			uint8_t getChipVersion();

			enum class OpMode : uint8_t
			{
				Sleep = 0,
				Standby=1,
				FsTx = 2,
				Tx = 3,
				FsRx = 4,
				RxContinuous = 5,
				RxSingle = 6,
				CAD = 7
			};

			int setOpMode(OpMode opMode);
			//OpMode getOpMode();

			int getConfigSize();
			int getConfig(uint8_t* buf);
			int setConfig(uint8_t* buf);
			int getStatus(uint8_t* buf);
			int bin_eval(uint8_t* buf, uint8_t* outbuf);

			int getPacketRSSI();
			int getPacketIRQFlags();
			int getPayloadLength();

			int getModemStatus();
			int getIRQFlags();
			int getIRQFlagMasks();
			int readStatus();

			int getRSSI();
			int getRxNbBytes();
			int getRxHeaderCnt();
			int getRxPacketCnt();
			int getFifo(uint8_t addr, int len, uint8_t* buf);
			int getRegHopChannel();

			int send(uint16_t len, const uint8_t* data); // carga datos y configura el modo de transmisión

			int receive(uint8_t continuous);
			int available();
			int pop(); // o de otra forma

			void isrDIO(uint32_t nPin);
			bool loop();
			void ISR();

		protected:

		private:
			void writeFifo(uint16_t len, uint8_t* data, uint16_t fifoAddr);
			void readFifo(uint16_t len, uint16_t fifoAddr);

			GPIO_TypeDef* _gpio0, *_gpio1, *_gpiorx, *_gpiotx, *_gpiocs, *_gpiorst;
			uint16_t _dio0, _dio1, _rxenpin, _txenpin, _cspin, _rstpin;

			int _workingFreq;
			uint16_t _preambLength, _regSymbTimeout, _lastUs;
			uint8_t _spreadingFactor, _syncWord, _bandwidth, _codingRate, _rxPayloadCrc, _payloadLength, _payloadMaxLength;
			uint8_t _freqHoppingPeriod, _txPowerReg, _regPaDac, _ocp, _chipVersion, _recv_flags, _PcktRSSI, _recvAddr;
			bool _implicitHeader, _isrFlag, m_waiting, m_pendingMsg, _dio0Ack;

			int _queueAvailable();
			void _queueCommand(uint8_t addr, uint8_t data);
			void _popCommand();
			uint16_t _tx_queue[16];
			uint8_t _tx_idx, _tx_size;
			uint8_t _tx_buf_len;
			uint8_t _tx_buf[129];

			uint8_t _rx_buf[256];
			uint16_t _rx_size;
			uint16_t _rx_idx;

			OpMode _opmode;
	};
}
#endif // LORA_H
