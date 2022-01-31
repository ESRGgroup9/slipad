#include "DEV_Config.h"

/**********************************************************************************************
* @brief  	DEV_ModuleInit
* @brief 	Module configuration, I2C protocol 
* @param
**********************************************************************************************/
uint8_t DEV_ModuleInit(void)
{
	if (!bcm2835_init()){
		printf("bcm2835 init failed! \r\n");
		return 1;
	}else {
        printf("bcm2835 init success! \r\n");
	}
	
	//I2C Config
    bcm2835_i2c_begin();
	bcm2835_i2c_setSlaveAddress(IIC_Addr);  //i2c address
	bcm2835_i2c_set_baudrate(100000);    //10K baudrate 10000
	bcm2835_i2c_setClockDivider(BCM2835_I2C_CLOCK_DIVIDER_626);
	
	return 0;
}

/**********************************************************************************************
* @brief  	bcm2835_IIC_Write
* @brief 	I2C Write
* @param	
**********************************************************************************************/
void bcm2835_IIC_Write(uint8_t add_, uint8_t data_)
{
	char Buf[2] = {0, 0};
	Buf[0] = add_;
	Buf[1] = data_;
	bcm2835_i2c_write(Buf, 2);
}

/**********************************************************************************************
* @brief  	DEV_ModuleInit()
* @brief 	I2C Read
* @param
**********************************************************************************************/
uint8_t bcm2835_IIC_Read(uint8_t add_)
{
	char Buf[1]={add_};
	bcm2835_i2c_write(Buf, 1);
	bcm2835_i2c_read(Buf, 1);
	return Buf[0];
}

/******************************************************************************
function:	
	Module exits, closes I2C
******************************************************************************/
void DEV_ModuleExit(void)
{
    bcm2835_i2c_end();
    bcm2835_close();
}


/************************************************/
