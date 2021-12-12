/******************** (C) COPYRIGHT tongxinmao.com ***************************
* �ļ���		: gpio.C
* ����   		: tcm123@126.com
* ��������  	: 2016.3.8
* ����			: GPIO����ͷ�ļ� 
* �汾			: V1.0
* ��ʷ��¼  	: ����
********************************************************************************/

/*--------------------------- ͷ�ļ����� --------------------------------------*/
#include "gpio.h"
/*-----------------------------------------------------------------------------*/


//------------------------------------------------------------------------
//------------------------------------------------------------------------
//--------------------         �궨��	      ------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------




//------------------------------------------------------------------------
//------------------------------------------------------------------------
//--------------------       ��������	      ------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------



//------------------------------------------------------------------------
//------------------------------------------------------------------------
//--------------------       ��������	      ------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------



//------------------------------------------------------------------------
//------------------------------------------------------------------------
//--------------------     ˽�к�������	    ------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------

 
void Led_Driver_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
		/* ʹ��LED��GPIO�ڵ�ʱ�� */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_LED_PORT1, ENABLE);
		/* ��������LED�Ƶ�����Ϊ���:PD12  PD13  PB4  PB5*/
		GPIO_InitStructure.GPIO_Pin   = LEDUp_PIN	| LEDDown_PIN;
		GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;					//--�������ģʽ
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 				//--GPIO��ת�ٶ�Ϊ50MHz
		GPIO_Init(LED_PORT1 , &GPIO_InitStructure);								//--������д���Ӧ��GPIO�Ĵ�����
}

  void I2C_GPIO_Config(void)
  {
 GPIO_InitTypeDef    GPIO_InitStructure;  

  GPIO_InitStructure.GPIO_Pin = I2C_SCL;          
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = I2C_SDA;           
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  }

void RFGPIOInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

        /* Configure PA8 RF_CE as Output push-pull -------------------------------*/
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_Init(GPIOA, &GPIO_InitStructure);


		/* Configure PB12 RF_Csn as Output push-pull -------------------------------*/
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_Init(GPIOB, &GPIO_InitStructure);

		/* Configure PB10 RF_IRQ as input */
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//GPIO_Mode_IPU;
        GPIO_Init(GPIOB, &GPIO_InitStructure);

		/* Configure PB11 RF_SDN as Output push-pull -------------------------------*/
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_Init(GPIOB, &GPIO_InitStructure);

}
/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : Configures the different GPIO ports.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GPIOInit(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE );
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE );
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE );

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0| GPIO_Pin_1;//LED
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0| GPIO_Pin_1;//KEY2 3
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;//KEY1
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5| GPIO_Pin_6| GPIO_Pin_7| GPIO_Pin_8| GPIO_Pin_9;//LED
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

   Led_Driver_Init();
   I2C_GPIO_Config();

   RFGPIOInit();

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE );
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE );
  
}
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//--------------------     ���к�������	    ------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------









/* end of Led_Driver.c -------------------------------------------------------------*/


