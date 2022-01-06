 
#ifndef __I2C_H
#define __I2C_H
//#include "BSP.h"
#include "stm32f10x.h"
#include "gpio.h" 
//============================= I2C�˿ڶ��� ===========================
#define I2C_PCLK          RCC_APB2Periph_GPIOA
#define I2C_PORT          GPIOA



#define I2C_SCL_H         I2C_PORT->BSRR = I2C_SCL	
#define I2C_SCL_L         I2C_PORT->BRR  = I2C_SCL 	
   
#define I2C_SDA_H         I2C_PORT->BSRR = I2C_SDA	 
#define I2C_SDA_L         I2C_PORT->BRR  = I2C_SDA	

#define READ_SDA      I2C_PORT->IDR  & I2C_SDA
//IO��������
#define SDA_IN()  {I2C_PORT->CRL&=0XF0FFFFFF;I2C_PORT->CRL|=8<<(4*6);}	  //pa6  
#define SDA_OUT() {I2C_PORT->CRL&=0XF0FFFFFF;I2C_PORT->CRL|=3<<(4*6);}
//===========================  ��������   =============================
void I2C_GPIO_Config(void);			 
void I2C_Start(void);				//����I2C��ʼ�ź�
void I2C_Stop(void);	  			//����I2Cֹͣ�ź�
void I2C_Send_Byte(u8 SendByte);			//I2C����һ���ֽ�
u8 I2C_Read_Byte(u8 ack);//I2C��ȡһ���ֽ�
u8 I2C_Wait_Ack(void); 				//I2C�ȴ�ACK�ź�
void I2C_Ack(void);					//I2C����ACK�ź�
void I2C_NAck(void);				//I2C������ACK�ź�
#endif  
 






