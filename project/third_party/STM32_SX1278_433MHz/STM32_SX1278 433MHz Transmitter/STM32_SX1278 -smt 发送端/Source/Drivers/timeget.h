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
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;

void TIM3_Int_Init(u16 arr,u16 psc);
void TIM3_IRQHandler(void);   //TIM3�ж�

#endif 
