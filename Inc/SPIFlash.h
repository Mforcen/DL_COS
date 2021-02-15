#ifndef SPIFLASH_H
#define SPIFLASH_H

#include "SPI_Device.h"
#include "mem_structs.hpp"
#include "stm32hal_libs.h"

namespace FwLogger
{
	// TODO (forcen#1#12/03/20): Change model flash driver
	class SPIFlash : public SPI_Device, public Module
	{
		public:
			SPIFlash(SPI_HandleTypeDef* hspi, GPIO_TypeDef* gpio, uint16_t pin, const char* moduleName="SPIFlash",
					 char bin_id=FLASH_BINID, uint8_t major=0, uint8_t minor=6);
			int writePage(uint8_t* data, uint16_t len, uint16_t page);
			int readPage(uint16_t page);
			int eraseSector(uint16_t page);
			int jedecId(uint8_t* data);

			void ISR();
			virtual void flashModuleISR();
			bool loop();

			int available();
			int peek();
			int pop();

			int bin_eval(uint8_t* buf, uint8_t* outbuf);
		protected:

		private:
			GPIO_TypeDef* _gpio;
			uint16_t _pin;

			enum Action : uint8_t
			{
				BusyCheck,
				WriteEnable,
				WELCheck,
				ProgramDataLoad,
				ProgramExecute,
				PageDataRead,
				Read,
				Erase,
				Delay
			};

			void _exec(Action a);
			void _isr(Action a);
			void _step();

			circular_buffer<8, Action> _actions;

			uint8_t tx_buffer[2052];

			uint8_t rx_buffer[2052];
			uint16_t rx_idx;
			uint16_t rx_size;

			uint16_t _page_idx;

			uint32_t _curr_time;
			uint32_t _delay_time;
			uint16_t _last_us;

			void _readStatus(uint8_t statreg);
			int _busyCheck();
			int _welCheck();
			void _writeEnable();
			void _programDataLoad();
			void _programExecute(uint16_t page);
			void _pageRead(uint16_t page);
			void _read();

			void _delay(int ms);

			bool _isr_launched = false;
	};
}
#endif // SPIFLASH_H
