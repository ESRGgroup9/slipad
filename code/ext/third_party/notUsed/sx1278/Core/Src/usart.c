/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
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

/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */
char Rx_Buffer[128];
char Tx_Buffer[128];
volatile uint8_t Rx_index = 0;
volatile uint8_t Tx_index = 0;

volatile uint8_t Tx_transmitting_str = 0;
volatile uint8_t Rx_receiving_str = 0;
/* USER CODE END 0 */

UART_HandleTypeDef huart3;

/* USART3 init function */

void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspInit 0 */

  /* USER CODE END USART3_MspInit 0 */
    /* USART3 clock enable */
    __HAL_RCC_USART3_CLK_ENABLE();

    __HAL_RCC_GPIOD_CLK_ENABLE();
    /**USART3 GPIO Configuration
    PD8     ------> USART3_TX
    PD9     ------> USART3_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /* USART3 interrupt Init */
    HAL_NVIC_SetPriority(USART3_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART3_IRQn);
  /* USER CODE BEGIN USART3_MspInit 1 */

  /* USER CODE END USART3_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspDeInit 0 */

  /* USER CODE END USART3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART3_CLK_DISABLE();

    /**USART3 GPIO Configuration
    PD8     ------> USART3_TX
    PD9     ------> USART3_RX
    */
    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_8|GPIO_PIN_9);

    /* USART3 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART3_IRQn);
  /* USER CODE BEGIN USART3_MspDeInit 1 */

  /* USER CODE END USART3_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
// set the interrupt for UART3 Rx
void UART_Receive(void)
{
	HAL_UART_Receive_IT(&huart3, (uint8_t*)&Rx_Buffer[Rx_index], 1);
}

void UART_Transmit(void)
{
	HAL_UART_Transmit_IT(&huart3, (uint8_t*)&Tx_Buffer[Tx_index], 1);
}

//implementation of UART ISR
void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart)
{
	if (huart->Instance == USART3) //current UART?
	{
		if(Rx_receiving_str)
		{
			// Received char is echoed
			Tx_Buffer[0] = Rx_Buffer[Rx_index];
			UART_Transmit();

			if((Rx_Buffer[Rx_index] == 0x0D) || ((Rx_index & (1<<7)) == 1))// 'CR'?
			{
				Rx_receiving_str = 0; 		// stop receiving
				Rx_Buffer[Rx_index] = 0;	// mark end of string
				Rx_index = -1; 						// Prepare for next reception. After following incr. Rx_Index = 0

				Tx_Buffer[1] = 0;					// Tx_transmission must end after sending this 'CR'
				Tx_transmitting_str = 1;
			}

			Rx_index++;
			UART_Receive();
		}
	}
}

//implementation of UART ISR
void HAL_UART_TxCpltCallback(UART_HandleTypeDef* huart)
{
	if (huart->Instance == USART3) //current UART?
	{
		if(Tx_transmitting_str == 0) 			// Transmitting a single char
			return;

		if(Tx_Buffer[Tx_index + 1] == 0)	// Is this the last caracter sent?
		{
			Tx_transmitting_str = 0;	// end of transmission
			Tx_index = 0; 						// prepare for next transmission
		}
		else
		{
			// print next char in Tx_Buffer
			Tx_index++;
			UART_Transmit();
		}
	}
}

void UART_putchar(char c)
{
	while(Tx_transmitting_str)// while Tx is busy
		;

	Tx_Buffer[0] = c;
	// Transmitting a single char
	UART_Transmit();
}

void UART_puts(const char *s)
{
	if((s == NULL) || (s[0] == 0)) // maybe in this application its not necessary
		return;

	while(Tx_transmitting_str)// while Tx is busy
		;

	strcpy(Tx_Buffer, s);
	Tx_transmitting_str = 1; // begin Tx_Buffer transmission
	UART_Transmit();
}
/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
