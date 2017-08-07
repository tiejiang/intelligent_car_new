/******************** (C) 1209 Lab **************************
 * 文件名  : UltrasonicWave.c
 * 描述    ：超声波测距模块，UltrasonicWave_Configuration（）函数
             初始化超声模块，UltrasonicWave_StartMeasure（）函数
			 启动测距，并将测得的数据通过串口1打印出来         
 * 实验平台：Mini STM32开发板  STM32F103ZET6
 * 硬件连接：------------------
 *          | PC2  - TRIG      |
 *          | PC3  - ECHO      |
 *           ------------------
 * 库版本  ：ST3.5.0
 *
 * 作者    ：2617 
*********************************************************************************/
#include "stm32f10x.h"
#include "ultrasonic.h"
#include "usart.h"
#include "timer.h"
#include "delay.h"
#include "sys.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_tim.h"

float Distance0=0;
float Distance1=0;      //计算出的距离   


/*
 * 函数名：UltrasonicWave_Configuration
 * 描述  ：超声波模块的初始化
 * 输入  ：无
 * 输出  ：无	
 */
void UltrasonicWave_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;	
	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);	//关闭jtag
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO, ENABLE);
    
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;					 //PC2接TRIG
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		     //设为推挽输出模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	         
  GPIO_Init(GPIOC, &GPIO_InitStructure);	                 //初始化外设GPIO 

  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;				     //PC3接ECH0
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;		 //设为输入
  GPIO_Init(GPIOC,&GPIO_InitStructure);						 //初始化GPIOA
	
	 //GPIOC.3	  中断线以及中断初始化配置
 	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource3);//----------------------P3

	EXTI_InitStructure.EXTI_Line=EXTI_Line3;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);		//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
		
			
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;			//使能按键所在的外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =1;	//抢占优先级2 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;					//子优先级2 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
}


void EXTI3_IRQHandler(void)
{
	u8 i=0;
	float result=0;
	delay_us(10);		                      //延时10us
  if(EXTI_GetITStatus(EXTI_Line3) != RESET)
	{		
		TIM_SetCounter(TIM2,0);
		TIM_Cmd(TIM2, ENABLE);                                     //开启时钟
	
		while(GPIO_ReadInputDataBit(GPIOC,ECHO_PIN));	             //等待低电平
		TIM_Cmd(TIM2, DISABLE);			                               //定时器2失能
		
		Distance0	=TIM_GetCounter(TIM2)*5*34/100.0;									 //计算距离&&UltrasonicWave_Distance<150
		// 		if(Distance0>2&&Distance0<450)
// 		{
// 			printf("距1:%3.2f \n",Distance0);
//  			delay_ms(10);
// 		}
		
		EXTI_ClearITPendingBit(EXTI_Line3);  												//清除EXTI7线路挂起位
}

}
/*
 * 函数名：UltrasonicWave_StartMeasure
 * 描述  ：开始测距，发送一个>10us的脉冲，然后测量返回的高电平时间
 * 输入  ：无
 * 输出  ：无	
 */
void UltrasonicWave_StartMeasure(void)
{
	TRIG_PIN=1; 		  					//送>10US的高电平RIG_PORT,TRIG_PIN
  delay_us(20);		                      //延时20US
	TRIG_PIN=0;
	delay_ms(50);
}

/******************* (C) 1209 Lab *****END OF FILE************/

