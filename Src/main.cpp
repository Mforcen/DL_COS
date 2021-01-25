/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usb_device.h"
#include "FwLogger.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

DAC_HandleTypeDef hdac;

I2C_HandleTypeDef hi2c1;
DMA_HandleTypeDef hdma_i2c1_rx;
DMA_HandleTypeDef hdma_i2c1_tx;

RTC_HandleTypeDef hrtc;

SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi2;
DMA_HandleTypeDef hdma_spi1_rx;
DMA_HandleTypeDef hdma_spi1_tx;

TIM_HandleTypeDef htim6;
TIM_HandleTypeDef htim7;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart3;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;

PCD_HandleTypeDef hpcd_USB_FS;

/* USER CODE BEGIN PV */

int16_t adc_data[7];

FwLogger::OS os;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/

/* USER CODE BEGIN PFP */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart);

uint16_t getUS();
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */


	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	os.hwinit();

	//MX_USB_DEVICE_Init();
	/* USER CODE BEGIN 2 */
	HAL_NVIC_SetPriority(RTC_Alarm_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(RTC_Alarm_IRQn);

	HAL_Delay(10);

	//HAL_ADCEx_Calibration_Start(&hadc1);

	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc_data, 7);

	FwLogger::OS::setOS(&os);

	HAL_TIM_Base_Start_IT(&htim6);

	HAL_TIM_Base_Start(&htim7);
	os.init();
	/* USER CODE END 2 */


	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{
		/* USER CODE END WHILE */
		os.loop();
		/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
}

/* USER CODE BEGIN 4 */

void _putchar(char c)
{
	FwLogger::OS::get().write(0, (void*) (&c), 1);
}

uint16_t getUS()
{
	return htim7.Instance->CNT;
}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef* hspi)
{
	if(hspi->Instance == SPI1)
		FwLogger::SPI_Device::CpltCallback();
}

void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef* hspi)
{
	if(hspi->Instance == SPI1)
		FwLogger::SPI_Device::CpltCallback();
}

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef* hspi)
{
	if(hspi->Instance == SPI1)
		FwLogger::SPI_Device::CpltCallback();
}

void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef* hrtc)
{
    FwLogger::OS::get().RTC_ISR();
}

void sdi12_Isr()
{
	FwLogger::OS::get().sdi12.timer_isr();
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == GPIO_PIN_0 || GPIO_Pin == GPIO_PIN_1) FwLogger::OS::get().radio.isrDIO(GPIO_Pin);
	else if(GPIO_Pin == GPIO_PIN_2 || GPIO_Pin == GPIO_PIN_3) FwLogger::OS::get().digital.ISR(GPIO_Pin);
	else if(GPIO_Pin < GPIO_PIN_4) FwLogger::OS::get().digital.ISR(GPIO_Pin);
	else if(GPIO_Pin == GPIO_PIN_6) FwLogger::OS::get().sdi12.pin_isr();
	else if(GPIO_Pin == GPIO_PIN_7) FwLogger::OS::get().sdi12.pin_isr();
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	while(1)
	{

	}
	/* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
	/* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
	   tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	/* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
