#ifndef __DEV_CONFIG_H__
#define __DEV_CONFIG_H__

#include <termio.h>
#include <stdio.h>
#include <bcm2835.h>

/**
* IIC 
**/
#define	IIC_Write(_add, _dat)		bcm2835_IIC_Write(_add, _dat)
#define	IIC_Read(_add)				bcm2835_IIC_Read(_add)
#define IIC_Addr					0x39

/**
 * delay x ms
**/
#define DEV_Delay_ms(__xms)         bcm2835_delay(__xms)

/*-----------------------------------------------------------------------------*/
void  bcm2835_IIC_Write(uint8_t add_, uint8_t data_);
uint8_t bcm2835_IIC_Read(uint8_t add_);

uint8_t DEV_ModuleInit(void);
void DEV_ModuleExit(void);

#endif