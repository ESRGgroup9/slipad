/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
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
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "SX1278.h"
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

/* USER CODE BEGIN PV */
char str[64];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
int _write(int file, char *ptr, int len) {
	int i;
	HAL_UART_Transmit(&huart3, (uint8_t*) ptr, len, 50);
	for (i = 0; i < len; i++) {
		ITM_SendChar(*ptr++);
	}
	return len;
}
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
SX1278_hw_t SX1278_hw;
SX1278_t SX1278;

int master;
int ret;

char buffer[512];

int message;
int message_length;
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

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI2_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
	master = HAL_GPIO_ReadPin(MODE_GPIO_Port, MODE_Pin);
	if (master == 1) {
		UART_puts("Mode: Master\r\n");
		HAL_GPIO_WritePin(MODE_GPIO_Port, MODE_Pin, GPIO_PIN_RESET);
	} else {
		UART_puts("Mode: Slave\r\n");
		HAL_GPIO_WritePin(MODE_GPIO_Port, MODE_Pin, GPIO_PIN_SET);
	}

	//initialize LoRa module
	SX1278_hw.dio0.port = DIO0_GPIO_Port;
	SX1278_hw.dio0.pin = DIO0_Pin;
	SX1278_hw.nss.port = NSS_GPIO_Port;
	SX1278_hw.nss.pin = NSS_Pin;
	SX1278_hw.reset.port = RESET_GPIO_Port;
	SX1278_hw.reset.pin = RESET_Pin;
	SX1278_hw.spi = &hspi2;

	SX1278.hw = &SX1278_hw;

	UART_puts("Configuring LoRa module\r\n");
	SX1278_init(&SX1278, 433000000, SX1278_POWER_17DBM, SX1278_LORA_SF_7,
			SX1278_LORA_BW_125KHZ, SX1278_LORA_CR_4_5, SX1278_LORA_CRC_EN, 10);
	UART_puts("Done configuring LoRaModule\r\n");

	if (master == 1) {
		ret = SX1278_LoRaEntryTx(&SX1278, 16, 2000);
		HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
	} else {
		ret = SX1278_LoRaEntryRx(&SX1278, 16, 2000);
		HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
	}

	if(ret == 1)
		UART_puts("EntryTX: Entering reception mode\r\n");
	else
		UART_puts("EntryTX: Timeout exceeded\r\n");

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if (master == 1)
	  {
		UART_puts("\r\nMaster ...\r\n");
		HAL_Delay(1000);
//		UART_puts("Sending package...\r\n");

		message_length = sprintf(buffer, "Hello %d", message);
		ret = SX1278_LoRaEntryTx(&SX1278, message_length, 2000);

		// lora entry TX return value - ret
		// 1 if entering reception mode
		// 0 if timeout was exceeded
		if(ret == 1)
			UART_puts("EntryTX: Entering reception mode\r\n");
		else
			UART_puts("EntryTX: Timeout exceeded\r\n");

//		snprintf(str, sizeof(str), "TX Entry retval: %d\r\n", ret);
//		UART_puts(str);

		snprintf(str, sizeof(str), "Sending [%s]\r\n", buffer);
		UART_puts(str);
		ret = SX1278_LoRaTxPacket(&SX1278, (uint8_t*) buffer, message_length, 2000);
		message += 1;

		if(ret == 1)
			UART_puts("TxPacket: Entering reception mode\r\n");
		else
			UART_puts("TxPacket: Timeout exceeded\r\n");

//		snprintf(str, sizeof(str), "TX retval: %d\r\n", ret);
//		UART_puts(str);
		UART_puts("Package sent...\r\n");

	  }
	  else
	  {
		UART_puts("\r\nSlave ...\r\n");
		HAL_Delay(800);
		UART_puts("Receiving package...\r\n");

		do{
			ret = SX1278_LoRaRxPacket(&SX1278);
		} while(ret == 0);

		snprintf(str, sizeof(str), "Received bytes: %d\r\n", ret);
		UART_puts(str);

		if (ret > 0) {
			SX1278_read(&SX1278, (uint8_t*) buffer, ret);

			snprintf(str, sizeof(str), "Content (%d): %s\r\n", ret, buffer);
			UART_puts(str);
		}
		UART_puts("Package received ...\r\n");
	  }

		//change mode
		if (GPIO_PIN_RESET == HAL_GPIO_ReadPin(MODE_GPIO_Port, MODE_Pin))
		{
			UART_puts("\r\nChanging mode...\r\n");
			master = ~master & 0x01;
			if (master == 1) {
				ret = SX1278_LoRaEntryTx(&SX1278, 16, 2000);
				HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
			} else {
				ret = SX1278_LoRaEntryRx(&SX1278, 16, 2000);
				HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
			}
			HAL_Delay(1000);
			while (GPIO_PIN_RESET == HAL_GPIO_ReadPin(MODE_GPIO_Port, MODE_Pin))
				;
		}
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 216;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART3;
  PeriphClkInitStruct.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
