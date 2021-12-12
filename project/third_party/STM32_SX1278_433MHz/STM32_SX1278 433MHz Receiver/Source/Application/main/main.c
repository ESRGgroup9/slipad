/**********************(C) COPYRIGHT tongxinmao.com*****************************
* 文件名			: main.c
* 作者            	: 10152203@qq.COM
* 创建日期          : 2016.3.5
* 描述				: 主文件	STM32F103C8T6+RF（SI443 SI4463 SX1278扩频）+OLED+串口+按键+温湿度传感器
* 历史记录          : 暂无
* 使用说明			：按键S1 短按切换无线是否应答，长按清空收发计数  S2：单次发送  S3:连续发送50*n次
					  发送的数据为温湿度数据，接收方收到后显示
********************************************************************************/

/*--------------------------- 头文件包含 --------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "function.h"
#include "oled.h"
#include "usart1.h"
#include "gpio.h"
#include "Delay.h"
#include "timeget.h"
#include "rf.h"
 
/*-----------------------------------------------------------------------------*/
/*
 * #pragma diag_suppress 77,550,1294,1295
 * #pragma diag_suppress 870
 */
#pragma diag_suppress 870
/*
 * ------------------------------------------------------------------------
 * ------------------------------------------------------------------------
 * --------------------         宏定义	      ------------------------------
 * ------------------------------------------------------------------------
 * ------------------------------------------------------------------------
 */

#define CMD_TEMP_HUM	1
#define CMD_ACK			2


/*
 * ------------------------------------------------------------------------
 * ------------------------------------------------------------------------
 * --------------------       变量定义	      ------------------------------
 * ------------------------------------------------------------------------
 * ------------------------------------------------------------------------
 */
u16 Counter=0,Msec=0;
u8 Counterflg=0;

/*
 * ------------------------------------------------------------------------
 * ------------------------------------------------------------------------
 * --------------------       函数声明	      ------------------------------
 * ------------------------------------------------------------------------
 * ------------------------------------------------------------------------
 */





void frameProcess( u8 *buf, u8 size )
{
	u8	cmd	= buf[0];
	u8	len	= buf[1];

	switch ( cmd )
	{
	case CMD_TEMP_HUM:
		if ( rfAck )
		{
			sendBuf[0]	= CMD_ACK;
			sendBuf[1]	= 0;
			RFSendData( sendBuf, 2 );
			msg( "回复应答..." );
		}
		/* 显示温湿度数据 */
		if ( len == 4 )
		{
			memcpy( &temp, buf + 2, 2 );
			memcpy( &hum, buf + 4, 2 );
			msg( "收到温湿度数据" );
		}
		break;

	case CMD_ACK:
		/* 屏幕显示应答成功 */

		msg( "收到应答" );
		break;

	default:
		break;
	}
}


void revieveProcess( u8 *buf, u8 size )
{
	u8 i=0;
	for(i=0;i<=size;i++)
	{
		printf("  %d",buf[i]);		
	}
		printf("\r\n");

}


/**************************************************************
 * 函数名  : main()
 * 描述    : 主函数
 *************************************************************/
int main( void )
{
		char buf[64] = "";
	/* --配置系统主频为72MHz */
	SystemInit();
	Delay_InitConfig( 72 );


	GPIOInit();	    //GPIO初始化
	OLED_Init();	//OLED初始化
	Uart1Init();	//串口初始化用于输出调试信息 9600bps 8N1

	TIM3_Int_Init(9999,7199);	
	RFInit();	 //射频模块初始化
	RFRxMode();	 //进入接收模式
	msg( "TIME 1S TEST" );
  getTempHum();  //获取温湿度数据
	while ( 1 )
	{
		int len;		
		sprintf( buf, "%s %s", rfName, rfAck ? "SLAVE" : "MASTER" );
		OLED_PXx16MixStr( 0, 0, buf );
		
		getTempHum();  //获取温湿度数据
		sprintf( buf, "温湿度%.1f %.1f%%", temp / 10.0, hum / 10.0 );		
		OLED_PXx16MixStr( 0, 1, buf );	
		
	if(Counterflg==1)	
		{		
			Counterflg=0;	
			printf("\r\n");			
			printf( "本机温度：%.1f   本机湿度：%.1f%%", temp / 10.0, hum / 10.0 );
			printf("\r\n");
		}
		
		len = RFRevData( revBuf );	  //接收无线数据
		if ( len > 0 )
		{
//			revieveProcess( revBuf, len ); //数据处理
			printf("\r\n");
			printf("当前收到秒数：%d",revBuf[0]);	
			printf("\r\n");
			if(revBuf[0]<10)  //清除一下显示花屏
			{
				sprintf( buf, "                ");			
				OLED_PXx16MixStr( 0, 2, buf );		
			}				
			sprintf( buf, "收到 %d  SEC %d", iRev, revBuf[0] );	
			OLED_PXx16MixStr( 0, 2, buf );
		}
	}



/*	while ( 1 )
	{	
			sprintf( buf, "%s %s", rfName, rfAck ? "MASTER  " : "SLAVE" );
			OLED_PXx16MixStr( 0, 0, buf );		
			if(Counterflg==1)	
			{	
						Counterflg=0;			
						sendBuf[0]	= Msec;
						sendBuf[1]	= 0;
						RFSendData( sendBuf, 2 );	
						sprintf( buf, "SEND %d SEC %d", iSend, Msec );	
						printf(" %d, %d",iSend,Msec);
						printf("\r\n");
						OLED_PXx16MixStr( 0, 2, buf );
			}
	}*/
	
/*	while ( 1 )
	{
		int len;

		keyProcess();  //按键检测处理
	    led_blink();   //led运行灯闪烁
 
		len = RFRevData( revBuf );	  //接收无线数据
		if ( len > 0 )
		{
			frameProcess( revBuf, len ); //数据处理
		}

		if(rfContinueSend())
		{	
			Delay_ms( 500 );	  
		}

		OledDisplay();	//刷新OLED显示
	}	*/
	
	
}




/*
 * ------------------------------------------------------------------------
 * ------------------------------------------------------------------------
 * --------------------     公有函数定义	    ------------------------------
 * ------------------------------------------------------------------------
 * ------------------------------------------------------------------------
 */




/* end of main.c -------------------------------------------------------------*/

