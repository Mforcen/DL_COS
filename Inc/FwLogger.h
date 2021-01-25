#ifndef FWLOGGER_H
#define FWLOGGER_H

#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <cctype>
#include <new>

#include "main.h"
#include "mem_structs.hpp"
#include "printf.h"
#include "KernelMQ.h"
#include "Connectivity.h"

#include "stm32hal_libs.h"
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
#include "Digital_Driver.h"

#define O_RDONLY 1
#define O_WRONLY 2
#define O_UPDATE 4

#define FWVMAJOR 0
#define FWVMINOR 1

#define FD_BUILTINS 3


extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc1;

extern DAC_HandleTypeDef hdac;

extern I2C_HandleTypeDef hi2c1;
extern DMA_HandleTypeDef hdma_i2c1_rx;
extern DMA_HandleTypeDef hdma_i2c1_tx;

extern RTC_HandleTypeDef hrtc;

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
extern int16_t adc_data[7];
extern uint8_t _UART_txing;

namespace FwLogger
{
	class OS
	{
		// TODO (forcen#1#): Add Low Power capabilities
		private:
			Allocator<128> _alloc;
			uint8_t _alloc_buf[8192];
			uint8_t _alloc_idx[64];
			void* _alloc_ownership[64];

		public:
			static OS& get();
			OS();
			static void setOS(OS* os);

			void hwinit();
			void init();

			void RTC_ISR();

			void loop();
			bool task_loop();
			void eval(uint8_t* buf, int fd);
			void bin_eval(uint8_t* ebuf, int length, int fd);
			void radio_eval();
			int open(char* path, int oflag);
			int read(int fd, void* buf, size_t count);
			int write(int fd, const void* buf, size_t count);
			int close(int fd);

			uint64_t time();
			eTSDB::Date timeETSDB();
			void setTime(RTC_DateTypeDef& date, RTC_TimeTypeDef& time);

			int16_t get_adc_raw(int channel);
			int get_adc_mv(int channel);
			int16_t get_bat_raw();
			int get_bat_mv();
			float get_battery();
			int getCharging();

			void enablePower(int enable);
			void enableLowPower(int enable);
			void setAlarm(RTC_TimeTypeDef& time);
			void setAlarm(int secs);
			int sleepFor(int s);
			void prepareSleep();
			void wakeUp();

			SPIFlash flash;
			//eTSDB::Driver etsdb;
			SDI12_Driver sdi12;
			VirtualMachine vm;
			Digital_Driver digital;

			LoRa radio;

			//SSD1306 disp;

		protected:

		private:
			void sleep();
			uint32_t _lastTaskTime;

			static OS* ptr;

			enum class FDType : uint8_t
			{
				None,
				File,
				TS,
				SPI,
				I2C,
				SDI12,
				//Port abstraction model
				SOCKET
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

			bool m_rxBin, m_rtcFlag, m_pendingTask, m_lpEnabled, m_pendingAlarm;

			int8_t _createFD(FDType type);
			void _deleteFD(int fd);

			FileDescriptor _fds[16];

			int printf_out;

			void saveConfig();
			void loadConfig();

			char m_name[32];
			bool m_init = false;
			uint32_t init_delay;
			uint32_t m_loop_time;
			uint32_t m_last_loop;
	};
}
#endif // FWLOGGER_H
