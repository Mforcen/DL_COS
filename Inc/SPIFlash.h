#ifndef SPIFLASH_H
#define SPIFLASH_H

#include "SPI_Device.h"
#include "mem_structs.hpp"

namespace FwLogger
{
	class SPIFlash : public SPI_Device
	{
		public:
			SPIFlash(SPI_HandleTypeDef* hspi, GPIO_TypeDef* gpio, uint16_t pin);
			int writePage(uint8_t* data, uint16_t len, uint32_t addr);
			int readPage(uint16_t len, uint32_t addr, uint8_t* buf);
			int readPage(uint16_t len, uint32_t addr);
			int eraseSector(uint32_t addr);
			int eraseChip();
			int jedecId(uint8_t* data);
			int powerDown();
			int wakeUp();

			void ISR();
			virtual void flashModuleISR();
			void poll();

			int available();
			int peek();
			int pop();
		protected:

		private:
			GPIO_TypeDef* _gpio;
			uint16_t _pin;

			enum Action
			{
				None,
				WENDelay,
				WriteEnable,
				WriteStart,
				Write,
				WaitBusy,
				SendReadAddr,
				Read,
				Erase
			} _action;

			uint8_t _rx_status[2];
			uint8_t _tx_status[2];

			struct WritingOp
			{
				uint16_t addr;
				uint16_t len;
			};

			circular_buffer<4, WritingOp> write_ops;

			uint8_t tx_buffer[330];

			uint8_t rx_buffer[330];
			uint8_t* ext_rx_buf;
			uint16_t rx_idx;
			uint16_t rx_size;

			uint16_t _last_us; // para añadir un poco de espera por si hace falta para que a la memoria le de tiempo de hacer cosas
			uint32_t _last_write_ms;
			uint16_t _max_delay_time;

			int writeEnable(); // añadir bsy check
			int writeDisable();

			void readStatus(int statusreg);
			void writeStatus(uint16_t status);
			bool _isr_launched = false;
	};
}
#endif // SPIFLASH_H
