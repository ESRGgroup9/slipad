//#include "stm32f10x_lib.h"
#include "sys.h"
#include "delay.h" //延时子函数
#include "usart.h"

void PWM_Init(u16 arr,u16 psc);
void Capture_Init(u16 arr,u16 psc);
//定时器2中断服务程序	 
void TIM2_IRQHandler(void);
