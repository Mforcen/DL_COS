#ifndef FWLOGGER_H
#define FWLOGGER_H

#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <cctype>
#include <new>

#include "mem_structs.hpp"
#include "printf.h"

#include "stm32f1xx_hal.h"
#include "pin_defs.h"
#include "errno.h"
#include "Log.h"

#include "VirtualMachine.h"
#include "SPIFlash.h"
#include "eTSDB.hpp"
#include "LoRa.h"
#include "SDI12_Driver.h"
#include "SSD1306.h"
#include "LinkLayer.h"

#define O_RDONLY 1
#define O_WRONLY 2
#define O_UPDATE 4

#define FWVMAJOR 0
#define FWVMINOR 1


extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc1;

extern DAC_HandleTypeDef hdac;

extern I2C_HandleTypeDef hi2c1;
extern DMA_HandleTypeDef hdma_i2c1_rx;
extern DMA_HandleTypeDef hdma_i2c1_tx;

extern RTC_HandleTypeDef hrtc;

extern SD_HandleTypeDef hsd;
extern DMA_HandleTypeDef hdma_sdio;

extern SPI_HandleTypeDef hspi1;
extern SPI_HandleTypeDef hspi2;
extern DMA_HandleTypeDef hdma_spi1_rx;
extern DMA_HandleTypeDef hdma_spi1_tx;

extern TIM_HandleTypeDef htim6; //sdi12 timer
extern TIM_HandleTypeDef htim7; //µS timer

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart3;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart1_tx;

extern PCD_HandleTypeDef hpcd_USB_FS;

extern circular_buffer<256> tx_buffer;
extern int16_t adc_data[5];
extern uint8_t _UART_txing;

namespace FwLogger
{
	class OS
	{
		// TODO (forcen#1#): Add Low Power capabilities
		public:
			static OS& get();
			OS();
			static void setOS(OS* os);

			void init();
			void push_rx(uint8_t c);

			void RTC_ISR();

			void loop();
			bool task_loop();
			void eval();
			void bin_eval(uint8_t* ebuf, int length, int fd);
			void radio_eval();
			int open(char* path, int oflag);
			int read(int fd, void* buf, size_t count);
			int poll(int fd);
			int write(int fd, const void* buf, size_t count);
			int close(int fd);

			eTSDB::Date time();

			int16_t get_adc_val(int channel);

			void enablePower(int enable);

			void ll_receive();

			SPIFlash flash;
			eTSDB::Driver etsdb;
			SDI12_Driver sdi12;
			VirtualMachine vm;
			LinkLayer lluart;

			//SSD1306 disp;

			LoRa radio;
		protected:

		private:
			void sleep();
			uint32_t _lastTaskTime;

			static OS* ptr;

			enum class Operation : uint8_t
			{
				None,
				Eval,
				BinEval,

				OpenFile,
				DownloadFile,
				Upload,
				Close,
				ReadNext,
				ListFiles,

				OpenHeader,
				ListTables,
				ReadTable,

				GetPage,
				SaveRow,
				//TestAppend,

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
				LL_UART,
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
				ProgramInitializer()
				{
					for(int i = 0; i < 16; ++i) name[i] = 0;
				}
                eTSDB::HeaderInitializer hi;
				uint8_t status;
				uint8_t tables;
                uint8_t name[16];
                uint8_t name_counter;
				uint8_t table_status;
				uint32_t stack_size;
			};

			int errno;
			circular_buffer<16, Task> _ops;

			fixed_string<128> rx_buffer;
			bool m_rxBin, m_rtcFlag, m_pendingTask, m_lpEnabled;
			enum class ParserStatus : uint8_t
			{
				Start,
				AsciiCommand,
				BinCommand,
			};

			ParserStatus m_pStatus;
			int m_pIndex;
			int m_pSize;
			int m_pLastRecv;
			float sdi12_test[2];

			int8_t _createFD(FDType type);
			void _deleteFD(int fd);

			FileDescriptor _fds[16];

			Allocator<128> _alloc;
			uint8_t _alloc_buf[4096];
			uint8_t _alloc_idx[32];
			uintptr_t _alloc_ownership[32];

			void saveConfig();
			void loadConfig();

			char m_name[32];
	};
}
#endif // FWLOGGER_H
