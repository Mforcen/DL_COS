/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
//#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void SystemClock_Config(void);
void MX_GPIO_Init(void);
void MX_DMA_Init(void);
void MX_ADC1_Init(void);
void MX_DAC_Init(void);
void MX_I2C1_Init(void);
void MX_I2C2_SMBUS_Init(void);
void MX_RTC_Init(void);
void MX_SDIO_SD_Init(void);
void MX_SPI1_Init(void);
void MX_SPI2_Init(void);
void MX_TIM6_Init(void);
void MX_TIM7_Init(void);
void MX_USART1_UART_Init(void);
void MX_USART3_UART_Init(void);
void Error_Handler(void);

extern unsigned char fw_standalone;

/* USER CODE BEGIN EFP */
void sdi12_Isr();
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define IO0_Pin GPIO_PIN_0
#define IO0_GPIO_Port GPIOC

#define IO1_Pin GPIO_PIN_1
#define IO1_GPIO_Port GPIOC

#define IO2_Pin GPIO_PIN_2
#define IO2_GPIO_Port GPIOC

#define IO3_Pin GPIO_PIN_3
#define IO3_GPIO_Port GPIOC

#define ADCBat_Pin GPIO_PIN_6
#define ADCBat_GPIO_Port GPIOA

#define Radio_rst_Pin GPIO_PIN_7
#define Radio_rst_GPIO_Port GPIOA

#define Radio_rxen_Pin GPIO_PIN_4
#define Radio_rxen_GPIO_Port GPIOC

#define Radio_txen_Pin GPIO_PIN_5
#define Radio_txen_GPIO_Port GPIOC

#define Radio_dio0_Pin GPIO_PIN_0
#define Radio_dio0_GPIO_Port GPIOB

#define Radio_dio1_Pin GPIO_PIN_1
#define Radio_dio1_GPIO_Port GPIOB

#define IO4_Pin GPIO_PIN_2
#define IO4_GPIO_Port GPIOB

#define Modem_en_Pin GPIO_PIN_10
#define Modem_en_GPIO_Port GPIOB

#define En12V_Pin GPIO_PIN_11
#define En12V_GPIO_Port GPIOB

#define En5V_Pin GPIO_PIN_12
#define En5V_GPIO_Port GPIOB

#define SDI12_0_Pin GPIO_PIN_6
#define SDI12_0_GPIO_Port GPIOC

#define SDI12_1_Pin GPIO_PIN_7
#define SDI12_1_GPIO_Port GPIOC

#define IO5_Pin GPIO_PIN_8
#define IO5_GPIO_Port GPIOC

#define CHG_Pin GPIO_PIN_9
#define CHG_GPIO_Port GPIOC

#define SDI12_Dir_Pin GPIO_PIN_8
#define SDI12_Dir_GPIO_Port GPIOA

#define Flash_nss_Pin GPIO_PIN_15
#define Flash_nss_GPIO_Port GPIOA

#define Modem_pwr_Pin GPIO_PIN_12
#define Modem_pwr_GPIO_Port GPIOC

#define Radio_nss_Pin GPIO_PIN_8
#define Radio_nss_GPIO_Port GPIOB

#define SPI2_NSS_Pin GPIO_PIN_9
#define SPI2_NSS_GPIO_Port GPIOB

#define STATUS_LED_Pin GPIO_PIN_2
#define STATUS_LED_Port GPIOD

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
