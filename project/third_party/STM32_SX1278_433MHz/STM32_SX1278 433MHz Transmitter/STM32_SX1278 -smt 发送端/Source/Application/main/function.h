
#ifndef __FUNCTION_H__
#define	__FUNCTION_H__

extern unsigned char	rfAck; //���߽��� Ӧ��/��Ӧ��ģʽ
extern  unsigned short temp,hum; //��ʪ��(*10)
extern unsigned short continueSendCnt; //����������
extern unsigned short continueSendIndex; //��ǰ������


void keyProcess( void );   //������⴦��
void getTempHum(void ); //��ȡ��ʪ��
void debug( const char* fmt, ... );	//���������Ϣ
void msg( char *msg );	//ͬʱ��OLED�ʹ��������Ϣ
void OledDisplay( void ); //oledˢ����ʾ
void led_blink(void);
char rfContinueSend(void);
#endif
