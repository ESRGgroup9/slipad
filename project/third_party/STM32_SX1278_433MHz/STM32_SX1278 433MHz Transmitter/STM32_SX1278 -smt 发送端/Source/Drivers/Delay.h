
#ifndef __DELAY_H
#define __DELAY_H	 
#include "stm32f10x.h"
void Delay_InitConfig(u8 SYSCLK);
void delay_us(u32 nus);
void delay_ms(u16 nms);
/* √Ë ˆ    : »Ìº˛—” ±£¨∫¡√Î*/
void  Delay_ms(uint16_t time_cn);
#endif 



