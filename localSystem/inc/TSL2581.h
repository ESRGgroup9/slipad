#ifndef _TSL2581_H_
#define _TSL2581_H_

#include <stdint.h> // uintx_t

bool Init_TSL2581(void);
void powerOnSensor(void);

void Reload_register(void);
void SET_Interrupt_Threshold(uint16_t min, uint16_t max);
void Read_Channel(void);
uint8_t Read_ID();
uint32_t calculateLux(uint16_t iGain);//, uint16_t tIntCycles);

#endif // !_TSL2581_H_
