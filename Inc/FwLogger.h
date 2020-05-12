#ifndef FWLOGGER_H
#define FWLOGGER_H

#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <new>

#include "mem_structs.hpp"
#include "printf.h"

#include "stm32f1xx_hal.h"
#include "VirtualMachine.h"
#include "SPIFlash.h"
#include "eTSDB.hpp"
#include "LoRa.h"
#include "pin_defs.h"

#include "SDI12_Driver.h"

#include "errno.h"

#define O_RDONLY 1
#define O_WRONLY 2
#define O_UPDATE 4


extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc3;
extern DMA_HandleTypeDef hdma_adc1;
extern DMA_HandleTypeDef hdma_adc3;

extern DAC_HandleTypeDef hdac;

extern I2C_HandleTypeDef hi2c1;
extern I2C_HandleTypeDef hi2c2;
extern DMA_HandleTypeDef hdma_i2c1_tx;

extern RTC_HandleTypeDef hrtc;

extern SD_HandleTypeDef hsd;

extern SPI_HandleTypeDef hspi1;
extern SPI_HandleTypeDef hspi2;
extern SPI_HandleTypeDef hspi3;
extern DMA_HandleTypeDef hdma_spi1_tx;

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim6; // SDI12 Timer
extern TIM_HandleTypeDef htim7; // uS timer

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern DMA_HandleTypeDef hdma_usart1_tx;
extern DMA_HandleTypeDef hdma_usart2_tx;
extern DMA_HandleTypeDef hdma_usart3_tx;

extern circular_buffer<256> tx_buffer;
extern int16_t adc1_data[6], adc3_data[6];
extern uint8_t _UART_txing;

namespace FwLogger
{
	class OS
	{
		public:
			static OS& get();
			OS();
			static void setOS(OS* os);

			void init();
			void push_rx(uint8_t c);

			void RTC_ISR();

			void loop();
			void eval();
			int open(char* path, int oflag);
			int read(int fd, void* buf, size_t count);
			int poll(int fd);
			int write(int fd, const void* buf, size_t count);
			int close(int fd);

			eTSDB::Date time();

			int16_t get_adc_val(int channel);

			void enablePower(int enable);

			SPIFlash flash;
			eTSDB::Driver etsdb;
			SDI12_Driver sdi12;
			VirtualMachine vm;

			LoRa radio;
		protected:

		private:
			static OS* ptr;

			enum class Operation : uint8_t
			{
				None,
				Eval,

				OpenFile,
				Upload,
				Close,
				ReadNext,

				OpenHeader,
				GetPage,
				SaveRow,
				TestAppend,

				LoadProgram
			};

			struct Task
			{
                Operation op;
                int8_t fd;
                uint16_t counter;
                void* buf;
                uint8_t name_buf[16];
			};

			enum class FDType : uint8_t
			{
				None,
				File,
				TS,
				SPI,
				UART,
				I2C,
				SDI12,
				Radio
			};

			struct FileDescriptor
			{
                FDType type;
                void* ptr;
                uint16_t bytes_read;
                uint8_t buf_idx;
                uint8_t* buf;
			};

			struct ProgramInitializer
			{
                eTSDB::HeaderInitializer hi;
				uint8_t status;
				uint8_t tables;
                uint8_t name[16];
                uint8_t name_counter;
				uint8_t table_status;
				uint32_t stack_size;
				uint32_t static_vars;
			};

			int errno;
			circular_buffer<16, Task> _ops; // implementar una priority queue

			fixed_string<128> rx_buffer;
			int parser_status;
			int parser_remaining;

			int8_t _createFD(FDType type);
			void _deleteFD(int fd);

			FileDescriptor _fds[16];

			Allocator _alloc;
			uint8_t _alloc_buf[4096];
			uint8_t _alloc_idx[32];
			uintptr_t _alloc_ownership[32];
			bool m_rtcFlag;
	};
}
#endif // FWLOGGER_H
