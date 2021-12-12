//#include "stm32f10x_lib.h"
#include "sys.h"
#include "delay.h" //��ʱ�Ӻ���
#include "usart.h"
#include "TIMECAN.h"
u16   IC2Value;
u16   DutyCycle;
u16   Frequency;
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void PWM_Init(u16 arr,u16 psc)
{		 					 
	//�˲������ֶ��޸� IO������  
	RCC->APB2ENR|=1<<0;    //  
	RCC->APB1ENR|=1<<2;       //TIM4 ʱ��ʹ�� 
	RCC->APB2ENR|=1<<3;    //ʹ��PORTBʱ�� 
	  
	GPIOB->CRL&=0XF0FFFFFF;//PB6 ���  
	GPIOB->CRL|=0X0B000000;//���ù������       
	GPIOB->ODR|=1<<6;//PB6 ����   
  
	TIM4->ARR=arr;//�趨�������Զ���װֵ   
	TIM4->PSC=psc;//Ԥ��Ƶ������Ƶ  

	TIM4->CCMR1|=7<<4;  //CH1 PWM2ģʽ      
	TIM4->CCMR1|=1<<3; //CH1 Ԥװ��ʹ��       
	TIM4->CCER|=1<<0;  //OC1  ���ʹ��      
	TIM4->CR1=0x0080;  //ARPEʹ��   

	TIM4->CR1|=0x01;    //ʹ�ܶ�ʱ�� 3  

	TIM4->CCR1   = 500;	 //ռ�ձȳ�ֵ ��  1440*50% �� 720              										  											  
} 

void Capture_Init(u16 arr,u16 psc)
{
	NVIC_InitTypeDef NVIC_InitStructure;	
	//�˲������ֶ��޸� IO������  
	RCC->APB2ENR|=1<<0;    //  
	RCC->APB1ENR|=1<<0;       //TIM2 ʱ��ʹ�� 
	RCC->APB2ENR|=1<<2;    //ʹ��PORTAʱ�� 
	   
 	TIM2->ARR=arr;  //�趨�������Զ���װֵ//�պ�1ms    
	TIM2->PSC=psc;  //Ԥ��Ƶ��,

	GPIOA->CRL&=0XFFFFFFF0;//PA0 ���  
	GPIOA->CRL|=0X00000004;//���ù������       
	GPIOA->ODR|=1<<0;//PA0 ����
	  
	TIM2->SMCR|=0x00D4; 
	//TIM2->SMCR|= 1<<5; //MSM=1 ��/��ģʽ 
	//TIM2->SMCR|= 5<<4; //TS=101 ����ѡ�� 
	//TIM2->SMCR|= 4<<0; //SMS=100 ��λģʽ

	TIM2->CCMR1|=1<<0;//CC1S=01 ѡ�������  
	TIM2->CCMR1|=3<<4; //IC1F=0011���������˲���
	TIM2->CCER|=0<<1; //CC1P=0	ѡ���н�ת������
	TIM2->CCMR1|=0<<2; //IC1PS=00 ���������Ƶ 
	TIM2->CCER|=1<<0; //CC1E=1 �������������ֵ������Ĵ�����

	TIM2->DIER|=1<<1;   //������²����ж�				
		   
	TIM2->CR1|=0x01;    //ʹ�ܶ�ʱ��2
//  NVIC_Init(1,3,TIM2_IRQChannel,2);//��ռ1�������ȼ�3����2	
	//TIM2�жϷ�������
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�01��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
}

//��ʱ��2�жϷ������	 
void TIM2_IRQHandler(void)
{ 		    		  			    
	IC2Value = TIM2->CCR1;//��ȡCCR1Ҳ������CC1IF��־λ
	printf("IC2Value = %d\r\n",1000000/IC2Value);			     	    					   
	TIM2->SR&=~(1<<1);//����жϱ�־λ 	    
}




