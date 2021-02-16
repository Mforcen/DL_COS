#ifndef SFS_H
#define SFS_H

#include <cstdint>
#include "SPIFlash.h"
#include "mem_structs.hpp"

namespace FwLogger
{
	class SFS : public SPIFlash
	{
		public:
			SFS(SPI_HandleTypeDef* hspi, GPIO_TypeDef* gpio, uint16_t pin, const char* moduleName="SPIFlash",
					 char bin_id=FLASH_BINID, uint8_t major=0, uint8_t minor=6);

			int init();

			int append(const uint8_t* buf, int len); // returns block pointer or <0 if error
			int startRead(int page);
			int mark_completed(int block); //marked as completed
			int mark_sended(int page); // mark entry as completed
			bool loop();

			void flashModuleISR();

			int _tailPage;
			int _tailBlock;
			int _markBlock;
			int _buf_len;

			int _read_len;
			int _read_idx;
			bool _read_sent;
			bool _read_closed;

			int available();
			int pop();
			int sent();
			int closed();


			enum Op
			{
				Initing,
				FindBlock,
				Append,
				Writing,
				MarkSended,
				Closing,
				Read
			};

			circular_buffer<8, Op> _ops;

			void _findBlockIt();

			void _exec();
			void _step();
			void _incPos();

		protected:

		private:
	};
}
#endif // SFS_H
