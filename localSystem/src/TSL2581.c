#include "TSL2581.h"
#include "DEV_Config.h"

// I2C address options
#define ADDR_VCC            0x29
#define ADDR_FLOAT          0x39   // Default address (pin left floating)
#define ADDR_GND            0x49
	 
//---------------------------------------------------
// x       xx      xxxxx 
//CMD TRANSACTION ADDRESS

#define COMMAND_CMD         0x80   
#define TRANSACTION         0x40    // read/write block protocol.
#define TRANSACTION_SPECIAL 0X60
//ADDRESS
#define CONTROL   	0x00
#define TIMING    	0x01
#define INTERRUPT 	0X02
#define THLLOW 		0x03
#define THLHIGH 	0X04
#define THHLOW 		0x05
#define THHHIGH 	0X06
#define ANALOG 		0X07

#define ID 			0X12
#define DATA0LOW 	0X14	// 1 10 1 0010 
#define DATA0HIGH 	0X15
#define DATA1LOW 	0X16
#define DATA1HIGH 	0X17
//---------------------------------------------------

#define ADC_EN				0X02
#define CONTROL_POWERON   	0x01
#define CONTROL_POWEROFF  	0x00
#define INTR_TEST_MODE 		0X30
#define INTR_INTER_MODE 	0X18//At least 8 cycles to stabilize, otherwise the interrupt will continue to maintain 0 

//TRANSACTION_SPECIAL
#define SPECIAL_FUN_RESER1 		0X00
#define SPECIAL_FUN_INTCLEAR 	0X01
#define SPECIAL_FUN_STOPMAN 	0X02
#define SPECIAL_FUN_STARTMAN 	0X03
#define SPECIAL_FUN_RESER2 		0X0F

//INTERRUPT
#define INTEGRATIONTIME_Manual 	0x00
#define INTEGRATIONTIME_2Z7MS 	0xFF
#define INTEGRATIONTIME_5Z4MS 	0xFE
#define INTEGRATIONTIME_51Z3MS 	0xED
#define INTEGRATIONTIME_100MS 	0xDB
#define INTEGRATIONTIME_200MS 	0xB6
#define INTEGRATIONTIME_400MS 	0x6C
#define INTEGRATIONTIME_688MS 	0x01

//ANALOG
#define GAIN_1X 	0x00
#define GAIN_8X 	0x01
#define GAIN_16X 	0x02
#define GAIN_111X 	0x03


#define LUX_SCALE 16 // scale by 2^16
#define RATIO_SCALE 9 // scale ratio by 2^9
//---------------------------------------------------
// Integration time scaling factors
//---------------------------------------------------
#define CH_SCALE 16 // scale channel values by 2^16

// Nominal 400 ms integration. 
// Specifies the integration time in 2.7-ms intervals
// 400/2.7 = 148
#define NOM_INTEG_CYCLE 148
//---------------------------------------------------
// Gain scaling factors
//---------------------------------------------------
#define CH0GAIN128X 107 // 128X gain scalar for Ch0
#define CH1GAIN128X 115 // 128X gain scalar for Ch1

//---------------------------------------------------
#define K1C 0x009A // 0.30 * 2^RATIO_SCALE
#define B1C 0x2148 // 0.130 * 2^LUX_SCALE
#define M1C 0x3d71 // 0.240 * 2^LUX_SCALE

#define K2C 0x00c3 // 0.38 * 2^RATIO_SCALE
#define B2C 0x2a37 // 0.1649 * 2^LUX_SCALE
#define M2C 0x5b30 // 0.3562 * 2^LUX_SCALE

#define K3C 0x00e6 // 0.45 * 2^RATIO_SCALE
#define B3C 0x18ef // 0.0974 * 2^LUX_SCALE
#define M3C 0x2db9 // 0.1786 * 2^LUX_SCALE

#define K4C 0x0114 // 0.54 * 2^RATIO_SCALE
#define B4C 0x0fdf // 0.062 * 2^LUX_SCALE
#define M4C 0x199a // 0.10 * 2^LUX_SCALE

#define K5C 0x0114 // 0.54 * 2^RATIO_SCALE
#define B5C 0x0000 // 0.00000 * 2^LUX_SCALE
#define M5C 0x0000 // 0.00000 * 2^LUX_SCALE
//---------------------------------------------------
#define MAX_LUX (((4095*B2C)+(1<<(LUX_SCALE-1)))>>LUX_SCALE)

static uint16_t Channel_0, Channel_1;

uint8_t Read_ID()
{
	return IIC_Read(COMMAND_CMD |TRANSACTION |ID);
}
 
/**********************************************************************************************
* @brief  	I2C_DEV_init()
* @brief   	powers on, set gain is 16, integration time is 688,5 ms
* @brief    This field selects the integration time for each conversion.
* @param    
**********************************************************************************************/
bool Init_TSL2581(void)
{ 
	if(DEV_ModuleInit(ADDR_FLOAT)==1)
		return false;

	powerOnSensor();
	
	IIC_Write(COMMAND_CMD | TIMING, INTEGRATIONTIME_688MS);  	//688,5 ms
	IIC_Write(COMMAND_CMD | CONTROL, ADC_EN | CONTROL_POWERON); //Every ADC cycle generates interrupt
	IIC_Write(COMMAND_CMD | INTERRUPT, INTR_INTER_MODE);		//TEST MODE
	IIC_Write(COMMAND_CMD | ANALOG, GAIN_1X);					//GAIN = 16

	return true;
}

void Exit_TSL2581(void)
{
	DEV_ModuleExit();
}

void powerOnSensor(void)
{
	/* write date from tsl2561 */
	IIC_Write(COMMAND_CMD | CONTROL,CONTROL_POWERON);	//power on
}

/**********************************************************************************************
* @brief  	Reload_register()
* @brief   Interrupts need to be maintained for several cycles
* @brief   When the interrupt bit is 0, reload the register
* @brief   Configure the special registers, clear the interrupt bits, and then re-enable the ADC
***********************************************************************************************/
void Reload_register(void)
{
	IIC_Write(COMMAND_CMD | TRANSACTION_SPECIAL | SPECIAL_FUN_INTCLEAR, INTR_INTER_MODE);
	IIC_Write(COMMAND_CMD | CONTROL, ADC_EN | CONTROL_POWERON); //Every ADC cycle generates interrupt
}

/**********************************************************************************************
* @brief  	SET_Interrupt_Threshold(uint32_t low,uint32_t high)
* @brief   	low and high max 2^16 = 65536
*   
* @param    This field selects the integration time for each conversion.
**********************************************************************************************/
void SET_Interrupt_Threshold(uint16_t min,uint16_t max)
{
	uint8_t DataLLow,DataLHigh,DataHLow,DataHHigh;
	DataLLow = min % 256;
	DataLHigh = min / 256;
	IIC_Write(COMMAND_CMD | THLLOW, DataLLow);  
	IIC_Write(COMMAND_CMD | THLHIGH, DataLHigh);  

	DataHLow = max % 256;
	DataHHigh = max / 256;
	IIC_Write(COMMAND_CMD | THHLOW, DataHLow);  
	IIC_Write(COMMAND_CMD | THHHIGH, DataHHigh); 
}	

/**********************************************************************************************
* @brief  	Read_Channel()
*     
* @brief   	read two ADC data
*      
**********************************************************************************************/
void Read_Channel()
{	
	uint8_t DataLow,DataHigh;
	DataLow = IIC_Read(COMMAND_CMD | TRANSACTION | DATA0LOW); 	// read channel 0 low byte	// command-> 1 10 10100
	DataHigh = IIC_Read(COMMAND_CMD | TRANSACTION | DATA0HIGH);	// read channel 0 high byte
	Channel_0 = 256 * DataHigh + DataLow ;
		
	DataLow = IIC_Read(COMMAND_CMD | TRANSACTION | DATA1LOW); 	// read channel 1 low byte
	DataHigh = IIC_Read(COMMAND_CMD | TRANSACTION | DATA1HIGH);	// read channel 1 high byte
	Channel_1 = 256 * DataHigh + DataLow ;
	
}

/**********************************************************************************************
* @brief  	calculateLux()
* @brief    Channel_0 and Channel_1 is for TSL2561_Read_Channel();
* @param   		iGain - gain, where 0:1X, 1:8X, 2:16X, 3:128X
* @param   	// tIntCycles - INTEG_CYCLES defined in Timing Register
**********************************************************************************************/
uint32_t calculateLux(uint16_t iGain)//, uint16_t tIntCycles)
{
	uint32_t chScale0;
	uint32_t chScale1;
	uint32_t channel1;
	uint32_t channel0;
	uint32_t temp;
	uint32_t ratio1 = 0;
	uint32_t ratio;
	uint32_t lux_temp;
	uint16_t b, m;

	// // No scaling if nominal integration (148 cycles or 400 ms) is used
	// if (tIntCycles == NOM_INTEG_CYCLE)
	// {
	// //     chScale0 = 65536;
	chScale0 = (1 << (CH_SCALE));
	// }
	// else
	// chScale0 = (NOM_INTEG_CYCLE << CH_SCALE) / tIntCycles;

	switch (iGain)
	{
		case 0: // 1x gain
		  chScale1 = chScale0; // No scale. Nominal setting
		  break;
		case 1: // 8x gain
		  chScale0 = chScale0 >> 3; // Scale/multiply value by 1/8
		  chScale1 = chScale0;
		  break;
		case 2: // 16x gain
		  chScale0 = chScale0 >> 4; // Scale/multiply value by 1/16
		  chScale1 = chScale0;
		  break;
		case 3: // 128x gain
		  chScale1 = chScale0 / CH1GAIN128X;
		  chScale0 = chScale0 / CH0GAIN128X;
		  break;
	}

	// Read Channel for ADC
	Read_Channel();
	// scale the channel values
	channel0 = (Channel_0 * chScale0) >>  CH_SCALE;
	channel1 = (Channel_1 * chScale1) >>  CH_SCALE;
	//printf(" C10= %ld  C11 =%ld \n ",channel0,channel1);
	// find the ratio of the channel values (Channel1/Channel0)
	if (channel0 != 0)
		ratio1 = (channel1 << (RATIO_SCALE + 1)) / channel0;
	
	ratio = (ratio1 + 1) >> 1;	  									 // round the ratio value

	if ((ratio >= 0X00) && (ratio <= K1C))
	{    
		b = B1C;    
		m = M1C;  
	}
	else if (ratio <= K2C)
	{    
		b = B2C;    
		m = M2C;  
	}
	else if (ratio <= K3C)
	{    
		b = B3C;    
		m = M3C;  
	}
	else if (ratio <= K4C)//276
	{    
		b = B4C;    
		m = M4C;  
	}
	else if (ratio > K5C)//276
	{    
		b = B5C;    
		m = M5C;  
	}

	temp = ((channel0 * b) - (channel1 * m));
	temp += (1 << (LUX_SCALE - 1));			// round lsb (2^(LUX_SCALE-1))
	//  temp = temp + 32768;

	if(channel0 == channel1 && channel0 == 4095)
        lux_temp=MAX_LUX;

	lux_temp = temp >> LUX_SCALE;			// strip off fractional portion
	return (lux_temp);		  							// Signal I2C had no errors
}
