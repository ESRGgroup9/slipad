//#include "stm32f10x_lib.h"
#include "sys.h"
#include "delay.h" //��ʱ�Ӻ���
#include "usart.h"

void PWM_Init(u16 arr,u16 psc);
void Capture_Init(u16 arr,u16 psc);
//��ʱ��2�жϷ������	 
void TIM2_IRQHandler(void);
