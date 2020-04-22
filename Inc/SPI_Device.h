#ifndef SPI_DEVICE_H
#define SPI_DEVICE_H

#include "stm32f1xx_hal.h"
#include "mem_structs.hpp"
#include "errno.h"
#include "Module.h"

uint16_t getUS();

namespace FwLogger
{
	class SPI_Device
	{
		public:
			static void CpltCallback(); //dejará status a Locked
			int startWrite(uint8_t *pdata, uint16_t len); //rellena buffer y inicia la transmisión por DMA
			int startRead(uint8_t* buf, uint16_t len); //configura la lectura del bus SPI
			int startTxRx(uint8_t* txbuf, uint8_t* rxbuf, uint16_t len);
			int try_lock();
			int unlock();
			virtual void ISR() = 0;

			enum SPIStatus
			{
				Idle,
				Locked,
				Transmitting,
				Receiving,
				TxRx
			};

			SPIStatus getStatus();

		protected:
			SPI_Device(SPI_HandleTypeDef* hspi);
			SPI_HandleTypeDef* _hspi;

		private:
			static SPIStatus _status;
			static SPI_Device* _locker;
	};
}

#endif // SPI_DEVICE_H
