
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "function.h"
#include "oled.h"
#include "usart1.h"
#include "gpio.h"
#include "SI7021.h"
#include "Delay.h"

#include "rf.h"

 u8	rfAck = 1;      /* 0��Ӧ��ģʽ  1Ӧ��ģʽ���յ��������ݺ�ظ��� */
 u16 temp=123,hum=456;
 unsigned short continueSendCnt=0; //����������
 unsigned short continueSendIndex=0;
/* ------------------------------------------------------------------------ */

void keyProcess( void )
{
	if ( GPIO_ReadInputDataBit( GPIOC, GPIO_Pin_13 ) == 0x00 ) /* S1 C13 */
	{
		delay_ms( 20 );
		if ( GPIO_ReadInputDataBit( GPIOC, GPIO_Pin_13 ) == 0x00 )
		{
			int i = 0;
			for ( i = 0; i < 10; i++ )
			{
				delay_ms( 100 );
				if ( GPIO_ReadInputDataBit( GPIOC, GPIO_Pin_13 ) != 0x00 )      /* �̰� */
				{
					rfAck = !rfAck;                                         /* �л�Ӧ��/��Ӧ��ģʽ */
					msg( rfAck ? "�л�ΪӦ��ģʽ" : "�л�Ϊ��Ӧ��ģʽ" );
					break;
				}
			}

			if ( i >= 10 )                                                          /* ���� */
			{
				msg( "clear ...." );
				iSend	= 0;
				iRev	= 0;
				while ( GPIO_ReadInputDataBit( GPIOC, GPIO_Pin_13 ) == 0x00 )
					;                                                       /* �ȴ������ͷ� */
				OLED_Fill( 0 );
			}
		}
	}

	if ( GPIO_ReadInputDataBit( GPIOA, GPIO_Pin_0 ) == 0x00 )                               /* S2 A0 */
	{
		delay_ms( 10 );
		getTempHum();
		if ( GPIO_ReadInputDataBit( GPIOA, GPIO_Pin_0 ) == 0x00 )
		{
			u8 len = 0;
			while ( GPIO_ReadInputDataBit( GPIOA, GPIO_Pin_0 ) == 0x00 )
				;                                                               /* �ȴ������ͷ� */
			sendBuf[0]	= 1;                                                    /* cmd: 1������ʪ������  2Ӧ��ɹ� */
			sendBuf[1]	= 4;    
			                                                /* ���ݳ��� */
			memcpy( &sendBuf[2], &temp, 2 );
			memcpy( &sendBuf[4], &hum, 2 );
			LedRed( On );
			len = RFSendData( sendBuf, 6 ); 
			if ( len > 0 )
				msg( "���η��ͳɹ�" );
			else
				msg( "���η���ʧ��" );
			/* msg((len>0?"���η��ͳɹ�":"���η���ʧ��")); */
			LedRed( Off );
		}
	}


	if ( GPIO_ReadInputDataBit( GPIOA, GPIO_Pin_1 ) == 0x00 )
	{
		delay_ms( 10 );
		if ( GPIO_ReadInputDataBit( GPIOA, GPIO_Pin_1 ) == 0x00 )
		{
		 
			while ( GPIO_ReadInputDataBit( GPIOA, GPIO_Pin_1 ) == 0x00 )
				;               /* �ȴ������ͷ� */
			
			   continueSendCnt+=50;

		
		}
	}
}

char rfContinueSend(void)
{
	 	   if(continueSendIndex>=continueSendCnt)return 0;
			continueSendIndex++;
				LedRed( On );
				getTempHum();
				sendBuf[0]	= 1;    /* cmd: 1������ʪ������  2Ӧ��ɹ� */
				sendBuf[1]	= 4;    /* ���ݳ��� */
				memcpy( &sendBuf[2], &temp, 2 );
				memcpy( &sendBuf[4], &hum, 2 );
				RFSendData( sendBuf, 6 );
				sprintf( (char *) sendBuf, "��������:%d/%d", continueSendIndex,continueSendCnt );
				msg( (char *) sendBuf );

				LedRed( Off );
				OledDisplay();
				return 1;
				
		 
}
/* ------------------------------------------------------------------------ */
void getTempHum( void )
{

		if(SI7021_Read( &temp, &hum ))
		{
		//msg("read si7021 ok");
//		debug( "�¶ȣ�%.1f ʪ�ȣ�%.1f%%", temp / 10.0, hum / 10.0 );
		 
		}else{
			  //msg("read si7021 err");
		}
	
}

void led_blink(void)
{
	static u32	count;
	static u8	flag = 0;

	if ( ++count > 200 )
	{
		count	= 0;
		flag	= !flag;
		/* LedRed(flag?On:Off); */
		LedGreen( flag );
		
	}
}

void debug( const char* fmt, ... )
{
	va_list args;
	va_start( args, fmt );
	vprintf( fmt, args );
	va_end( args );
}

/*
 * ------------------------------------------------------------------------
 * ------------------------------------------------------------------------
 * --------------------     ˽�к�������	    ------------------------------
 */
void msg( char *msg )                                   /* ���Ҫ��ʾ���ģ�OLED�����ֿ��������Ӧ���ݣ�Ӣ������Ҫ */
{
	static u8 count=0;
	char buf[8];
	OLED_PXx16MixStr( 0, 3, "                " );   /* 16����ĸ */
    sprintf(buf,"%d:",++count%10);
    OLED_PXx16MixStr(0,3,buf);
	OLED_PXx16MixStr( 14, 3, msg );
	debug( msg );
 
}															                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            


void OledDisplay( void )
{
	char buf[64] = "";

	sprintf( buf, "%s %s", rfName, rfAck ? "����Ӧ��  " : "������Ӧ��" );

	OLED_PXx16MixStr( 0, 0, buf );

	sprintf( buf, "��ʪ��%.1f %.1f%%", temp / 10.0, hum / 10.0 );
	OLED_PXx16MixStr( 0, 1, buf );

	sprintf( buf, "��%d ��%d", iSend, iRev );

	OLED_PXx16MixStr( 0, 2, buf );
}

