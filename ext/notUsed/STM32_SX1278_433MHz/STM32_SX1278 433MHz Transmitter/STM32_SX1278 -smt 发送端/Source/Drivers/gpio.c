/******************** (C) COPYRIGHT tongxinmao.com ***************************
* 文件名		: gpio.C
* 作者   		: tcm123@126.com
* 创建日期  	: 2016.3.8
* 描述			: GPIO驱动头文件 
* 版本			: V1.0
* 历史记录  	: 暂无
********************************************************************************/

/*--------------------------- 头文件包含 --------------------------------------*/
#include "gpio.h"
/*-----------------------------------------------------------------------------*/


//------------------------------------------------------------------------
//------------------------------------------------------------------------
//--------------------         宏定义	      ------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------




//------------------------------------------------------------------------
//------------------------------------------------------------------------
//--------------------       变量定义	      ------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------



//------------------------------------------------------------------------
//------------------------------------------------------------------------
//--------------------       函数声明	      ------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------



//------------------------------------------------------------------------
//------------------------------------------------------------------------
//--------------------     私有函数定义	    ------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------

 
void Led_Driver_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
		/* 使能LED灯GPIO口的时钟 */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_LED_PORT1, ENABLE);
		/* 配置所有LED灯的引脚为输出:PD12  PD13  PB4  PB5*/
		GPIO_InitStructure.GPIO_Pin   = LEDUp_PIN	| LEDDown_PIN;
		GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;					//--推免输出模式
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 				//--GPIO翻转速度为50MHz
		GPIO_Init(LED_PORT1 , &GPIO_InitStructure);								//--将配置写入对应的GPIO寄存器中
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
//--------------------     公有函数定义	    ------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------









/* end of Led_Driver.c -------------------------------------------------------------*/


