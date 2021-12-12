//#include "stm32f10x_lib.h"
#include "sys.h"
#include "delay.h" //延时子函数
#include "usart.h"
#include "TIMECAN.h"
u16   IC2Value;
u16   DutyCycle;
u16   Frequency;
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void PWM_Init(u16 arr,u16 psc)
{		 					 
	//此部分需手动修改 IO口设置  
	RCC->APB2ENR|=1<<0;    //  
	RCC->APB1ENR|=1<<2;       //TIM4 时钟使能 
	RCC->APB2ENR|=1<<3;    //使能PORTB时钟 
	  
	GPIOB->CRL&=0XF0FFFFFF;//PB6 输出  
	GPIOB->CRL|=0X0B000000;//复用功能输出       
	GPIOB->ODR|=1<<6;//PB6 上拉   
  
	TIM4->ARR=arr;//设定计数器自动重装值   
	TIM4->PSC=psc;//预分频器不分频  

	TIM4->CCMR1|=7<<4;  //CH1 PWM2模式      
	TIM4->CCMR1|=1<<3; //CH1 预装载使能       
	TIM4->CCER|=1<<0;  //OC1  输出使能      
	TIM4->CR1=0x0080;  //ARPE使能   

	TIM4->CR1|=0x01;    //使能定时器 3  

	TIM4->CCR1   = 500;	 //占空比初值 ＝  1440*50% ＝ 720              										  											  
} 

void Capture_Init(u16 arr,u16 psc)
{
	NVIC_InitTypeDef NVIC_InitStructure;	
	//此部分需手动修改 IO口设置  
	RCC->APB2ENR|=1<<0;    //  
	RCC->APB1ENR|=1<<0;       //TIM2 时钟使能 
	RCC->APB2ENR|=1<<2;    //使能PORTA时钟 
	   
 	TIM2->ARR=arr;  //设定计数器自动重装值//刚好1ms    
	TIM2->PSC=psc;  //预分频器,

	GPIOA->CRL&=0XFFFFFFF0;//PA0 输出  
	GPIOA->CRL|=0X00000004;//复用功能输出       
	GPIOA->ODR|=1<<0;//PA0 上拉
	  
	TIM2->SMCR|=0x00D4; 
	//TIM2->SMCR|= 1<<5; //MSM=1 主/从模式 
	//TIM2->SMCR|= 5<<4; //TS=101 触发选择 
	//TIM2->SMCR|= 4<<0; //SMS=100 复位模式

	TIM2->CCMR1|=1<<0;//CC1S=01 选择输入端  
	TIM2->CCMR1|=3<<4; //IC1F=0011配置输入滤波器
	TIM2->CCER|=0<<1; //CC1P=0	选择有交转换边沿
	TIM2->CCMR1|=0<<2; //IC1PS=00 配置输入分频 
	TIM2->CCER|=1<<0; //CC1E=1 允许捕获计数器的值到捕获寄存器中

	TIM2->DIER|=1<<1;   //允许更新捕获中断				
		   
	TIM2->CR1|=0x01;    //使能定时器2
//  NVIC_Init(1,3,TIM2_IRQChannel,2);//抢占1，子优先级3，组2	
	//TIM2中断分组配置
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级01级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
}

//定时器2中断服务程序	 
void TIM2_IRQHandler(void)
{ 		    		  			    
	IC2Value = TIM2->CCR1;//读取CCR1也可以清CC1IF标志位
	printf("IC2Value = %d\r\n",1000000/IC2Value);			     	    					   
	TIM2->SR&=~(1<<1);//清除中断标志位 	    
}




