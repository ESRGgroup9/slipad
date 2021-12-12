

 
#include "sx1276-LoRa.h"
#include "Delay.h"
#include "SPI.h"
#include "rf.h"
#include "function.h"

 
 
const char *rfName = "SX1278";
 


u16	iSend, iRev;	//无线发送 接收计数
u8	sendBuf[64];	//发送缓冲区
u8	revBuf[128];	//接收缓冲区



//射频模块初始化
void RFInit( void )
{

	SPI2_Init();

 
 
	RFM96_LoRaEntryRx();
 
}


//射频模块进入接收模式
void RFRxMode( void )
{
 
	RFM96_LoRaEntryRx();
 
}


//射频模块接收数据
u8 RFRevData( u8 *buf )
{
	u8 length = 0;

 
	
	if ( GPIO_ReadInputDataBit( GPIOB, RF_IRQ_PIN ) ) //收到数据高电平中断

	{
		msg("RF Received");

 	 	length=RFM96_LoRaRxPacket(revBuf);
 
		RFRxMode();

	}


	if(length>0)iRev++;

	return(length);
}


u8 RFSendData( u8 *buf, u8 size )
{
	int ret = 0;
 
   	ret=RFM96_LoRaEntryTx(size);
		ret=RFM96_LoRaTxPacket(buf,size);	 
 


	Delay_ms( 10 );
	RFRxMode();
	 

	if ( ret > 0 ) iSend++;
	if ( iSend > 99 ) iSend=0;


	return(ret);
}

