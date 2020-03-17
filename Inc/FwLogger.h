#ifndef FWLOGGER_H
#define FWLOGGER_H

#include <cstdint>
#include <cstring>
#include "mem_structs.hpp"
#include "tinyscript.h"
#include "printf.h"

#include "stm32f1xx_hal.h"
#include "SPIFlash.h"
#include "eTSDB.hpp"
#include "LoRa.h"

#include "SDI12_Driver.h"

#include "errno.h"

#define ADC_SE_0	0
#define ADC_SE_1	1
#define ADC_SE_2	2
#define ADC_SE_3	3
#define ADC_SE_4	4
#define ADC_SE_5	5
#define ADC_DE_0	6
#define ADC_DE_1	7
#define ADC_DE_2	8
#define ADC_DE_3	9
#define ADC_DE_4	10
#define ADC_DE_5	11

#define IO0 GPIOE,GPIO_PIN_0
#define IO1 GPIOE,GPIO_PIN_1
#define IO2 GPIOE,GPIO_PIN_2
#define IO3 GPIOE,GPIO_PIN_3
#define IO4 GPIOE,GPIO_PIN_4
#define IO5 GPIOE,GPIO_PIN_5
#define IO6 GPIOE,GPIO_PIN_6
#define IO7 GPIOE,GPIO_PIN_7
#define IO8 GPIOE,GPIO_PIN_8
#define IO9 GPIOE,GPIO_PIN_10
#define IO10 GPIOE,GPIO_PIN_11
#define IO11 GPIOE,GPIO_PIN_12
#define IO12 GPIOE,GPIO_PIN_13
#define IO13 GPIOE,GPIO_PIN_14
#define IO14 GPIOE,GPIO_PIN_15

#define IO20 GPIOD,GPIO_PIN_0
#define IO21 GPIOD,GPIO_PIN_1
#define IO22 GPIOD,GPIO_PIN_3
#define IO23 GPIOD,GPIO_PIN_4
#define IO24 GPIOD,GPIO_PIN_7
#define IO25 GPIOD,GPIO_PIN_10
#define IO26 GPIOD,GPIO_PIN_11
#define IO27 GPIOD,GPIO_PIN_13
#define IO28 GPIOD,GPIO_PIN_14
#define IO29 GPIOD,GPIO_PIN_15
#define IO210 GPIOB,GPIO_PIN_12

#define SDI12_1 GPIOC,GPIO_PIN_14
#define SDI12_2 GPIOC,GPIO_PIN_15
#define SDI12_OE GPIOA,GPIO_PIN_15

#define FLASH_CS IO20

#define LORA_CS		IO22
#define LORA_DIO0	IO23
#define LORA_DIO1	IO24
#define LORA_DIO2	IO25
#define LORA_DIO3	IO26
#define LORA_TXEN	IO27
#define LORA_RXEN	IO28
#define LORA_RST	IO21

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

extern circular_buffer tx_buffer;
extern int16_t adc1_data[6], adc3_data[6];

class FwLogger
{
	public:
		//static FwLogger* getInstance();
		FwLogger();

		void init();

		void loop();
		void eval(uint8_t* input_str);
		int open(char* path, int oflag);
		int read(int fd, void* buf, size_t count);
		int write(int fd, void* buf, size_t count);

		int16_t get_adc_val(int channel);

		void enablePower(int enable);

		SPIFlash flash;
		eTSDB::Driver etsdb;
		SDI12_Driver sdi12;

		LoRa radio;
	protected:

	private:
		uint8_t ts_inter_arena[4096];
		int errno;
};

#endif // FWLOGGER_H
