#ifndef PIN_DEFS_H_INCLUDED
#define PIN_DEFS_H_INCLUDED

#include "main.h"

#define ADC_SE_0	0
#define ADC_SE_1	1
#define ADC_SE_2	2
#define ADC_SE_3	3
#define ADC_SE_4	4
#define ADC_SE_5	5
#define ADC_SE_6	6

#define IO0 GPIOC,GPIO_PIN_0
#define IO1 GPIOC,GPIO_PIN_1
#define IO2 GPIOC,GPIO_PIN_2
#define IO3 GPIOC,GPIO_PIN_3
#define IO4 GPIOB,GPIO_PIN_4
#define IO5 GPIOC,GPIO_PIN_8

#define V12En GPIOB,GPIO_PIN_11
#define V5En GPIOB,GPIO_PIN_12

#define SDI12_0 SDI12_0_GPIO_Port,SDI12_0_Pin
#define SDI12_1 SDI12_1_GPIO_Port,SDI12_1_Pin
#define SDI12_DIR SDI12_Dir_GPIO_Port,SDI12_Dir_Pin

#define FLASH_CS Flash_nss_GPIO_Port,Flash_nss_Pin

#define LORA_CS		Radio_nss_GPIO_Port,Radio_nss_Pin
#define LORA_DIO0	Radio_dio0_GPIO_Port,Radio_dio0_Pin
#define LORA_DIO1	Radio_dio1_GPIO_Port,Radio_dio1_Pin
#define LORA_TXEN	Radio_txen_GPIO_Port,Radio_txen_Pin
#define LORA_RXEN	Radio_rxen_GPIO_Port,Radio_rxen_Pin
#define LORA_RST	Radio_rst_GPIO_Port,Radio_rst_Pin

#endif // PIN_DEFS_H_INCLUDED
