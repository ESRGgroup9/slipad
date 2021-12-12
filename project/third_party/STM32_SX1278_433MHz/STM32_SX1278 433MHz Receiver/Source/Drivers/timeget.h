#ifndef __TIMEGET_H
#define __TIMEGET_H	
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "function.h"
#include "oled.h"
#include "usart1.h"
#include "gpio.h"
#include "Delay.h"		
#include "stm32f10x_tim.h"
//本程序只供学习使用，未经作者许可，不得用于其它任何用途

void TIM3_Int_Init(u16 arr,u16 psc);
void TIM3_IRQHandler(void);   //TIM3中断

#endif 
