
#ifndef __FUNCTION_H__
#define	__FUNCTION_H__

extern unsigned char	rfAck; //无线接收 应答/非应答模式
extern  unsigned short temp,hum; //温湿度(*10)
extern unsigned short continueSendCnt; //连续发包数
extern unsigned short continueSendIndex; //当前发包数


void keyProcess( void );   //按键检测处理
void getTempHum(void ); //获取温湿度
void debug( const char* fmt, ... );	//串口输出信息
void msg( char *msg );	//同时在OLED和串口输出信息
void OledDisplay( void ); //oled刷新显示
void led_blink(void);
char rfContinueSend(void);
#endif
