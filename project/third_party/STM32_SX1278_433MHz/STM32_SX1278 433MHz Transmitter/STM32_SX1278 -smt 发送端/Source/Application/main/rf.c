

 
#include "sx1276-LoRa.h"
#include "Delay.h"
#include "SPI.h"
#include "rf.h"
#include "function.h"

 
 
const char *rfName = "SX1278";
 


u16	iSend, iRev;	//���߷��� ���ռ���
u8	sendBuf[64];	//���ͻ�����
u8	revBuf[128];	//���ջ�����



//��Ƶģ���ʼ��
void RFInit( void )
{

	SPI2_Init();

 
 
	RFM96_LoRaEntryRx();
 
}


//��Ƶģ��������ģʽ
void RFRxMode( void )
{
 
	RFM96_LoRaEntryRx();
 
}


//��Ƶģ���������
u8 RFRevData( u8 *buf )
{
	u8 length = 0;

 
	
	if ( GPIO_ReadInputDataBit( GPIOB, RF_IRQ_PIN ) ) //�յ����ݸߵ�ƽ�ж�

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

