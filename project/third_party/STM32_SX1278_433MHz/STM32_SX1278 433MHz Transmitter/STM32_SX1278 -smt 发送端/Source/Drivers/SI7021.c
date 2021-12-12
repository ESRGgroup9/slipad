/******************** (C) COPYRIGHT tongxinmao.com ***************************
* �ļ���		: si7021.C
* ����   		: tcm123@126.com
* ��������  	: 2016.3.8
* ����			: ��ʪ�ȴ����������ļ� 
* �汾			: V1.0
* ��ʷ��¼  	: ����
********************************************************************************/
#include "SI7021.h"
#include "I2C.h"
#include "Delay.h"
#include "function.h"

//===================================================================== 
//�� �� ��:	SI7021_Read_Data
//��    ��: ��SI7021��ȡһ������ 
//��ڲ���:
//���ڲ���:
//�� �� ֵ:	
//=====================================================================
u8 SI7021_Read_Data(u8 Model,u16 *temp) 
{ 
	u8 RX_Buffer[2];
	I2C_Start(); //��ʼ�ź�              
	I2C_Send_Byte(SALVE_ADDR);
    if(I2C_Wait_Ack()==1){
		return 0;
	}
	I2C_Send_Byte(Model);
	I2C_Wait_Ack();
  delay_ms(20);
	I2C_Start(); //��ʼ�ź� 		 	//���·���ʼ�ź�
	I2C_Send_Byte(READ_CDM);
	I2C_Wait_Ack();
	RX_Buffer[0]=I2C_Read_Byte(1);
	RX_Buffer[1]=I2C_Read_Byte(0);
	I2C_Stop();
	*temp=((RX_Buffer[0]*256)+RX_Buffer[1]) & ~3;

	return 1;
}
//===================================================================== 
//�� �� ��:	SI7021_Read_Data
//��    ��: ��SI7021��ȡ��ʪ��
//��ڲ���:temp:�¶�ֵ(��Χ:0~50��)	 humi:ʪ��ֵ(��Χ:20%~90%)
//���ڲ���:
//�� �� ֵ:	
//=====================================================================
u8 SI7021_Read(u16 *temp,u16 *humi)  
{ 
	u16 Value;
	if(!SI7021_Read_Data(TEMP_NOHOLD_MASTER, &Value ))return 0;
  *temp= ((((long)Value)*1757)>>16) - 469;
  if(!SI7021_Read_Data(HUMI_NOHOLD_MASTER, &Value ))return 0;
  *humi= ((((long)Value)*625)>>15) - 60;
  
  return 1;
}


