
#ifndef __OLED_H__
#define __OLED_H__

/*----------------------------------------------------------------------------*
 * ����ͷ�ļ�                                                                 *
 *----------------------------------------------------------------------------*/
//#include "stm32f10x_lib.h"
#include "stm32f10x.h"
/*----------------------------------------------------------------------------*
 * �궨��                                                                     *
 *----------------------------------------------------------------------------*/
#define LED_IMAGE_WHITE       1
#define LED_IMAGE_BLACK       0

#define LED_MAX_ROW_NUM      64
#define LED_MAX_COLUMN_NUM  128

#define VOID void
#define UCHAR8 unsigned char
#define CHAR8 char  
#define USHORT16 unsigned short
#define SHORT16 short
#define ULONG32 unsigned long int  
#define LONG32 long int
#define ULONG64 unsigned long long int
#define LONG64 long long int
#define TRUE 1
#define FALSE 0




/*----------------------------------------------------------------------------*
 * ȫ�ֱ���                                                                   *
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
 * �ڲ�����ԭ��                                                               *
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
 * �ⲿ����ԭ��                                                               *
 *----------------------------------------------------------------------------*/
 
extern VOID OLED_Init(VOID);
extern VOID OLED_Fill(UCHAR8 ucData);
extern VOID OLED_SetPos(UCHAR8 ucIdxX, UCHAR8 ucIdxY); 
extern VOID OLED_P6x8Char(UCHAR8 ucIdxX,UCHAR8 ucIdxY,UCHAR8 ucData);   //��ʾһ��6x8��׼ASCII�ַ�
extern VOID OLED_P6x8Str(UCHAR8 ucIdxX,UCHAR8 ucIdxY,CHAR8 ucDataStr[]);
extern VOID OLED_P8x16Str(UCHAR8 ucIdxX,UCHAR8 ucIdxY,CHAR8 ucDataStr[]);  //д��һ��8x16��׼ASCII�ַ���
extern VOID OLED_P14x16Str(UCHAR8 ucIdxX,UCHAR8 ucIdxY,CHAR8 ucDataStr[]);	  //д��һ��14x16�������ַ������ַ���������躬�д��֣�
extern VOID OLED_PXx16MixStr(UCHAR8 ucIdxX, UCHAR8 ucIdxY, CHAR8 ucDataStr[]); //���14x16���ֺ��ַ�����ַ��� ���ַ���������躬�д��֣�
extern VOID OLED_Fill(UCHAR8 ucData);
extern VOID OLED_PrintChar(UCHAR8 ucIdxX, UCHAR8 ucIdxY, CHAR8 cData);
extern VOID OLED_PrintShort(UCHAR8 ucIdxX, UCHAR8 ucIdxY, SHORT16 sData);
extern VOID OLED_PrintImage(UCHAR8 *pucTable, USHORT16 usRowNum, USHORT16 usColumnNum);
#endif

