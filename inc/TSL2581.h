#ifndef _TSL2581_H_
#define _TSL2581_H_

#include <stdint.h> // uintx_t

extern bool Init_TSL2581(void);
extern void powerOnSensor(void);

extern void Reload_register(void);
extern void SET_Interrupt_Threshold(uint16_t min, uint16_t max);
extern void Read_Channel(void);
extern uint8_t Read_ID();
extern uint32_t calculateLux(uint16_t iGain);//, uint16_t tIntCycles);

#endif // !_TSL2581_H_
