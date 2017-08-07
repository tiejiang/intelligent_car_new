/******************** (C) 1209 Lab **************************
 * �ļ���  : UltrasonicWave.c
 * ����    �����������ģ�飬UltrasonicWave_Configuration��������
             ��ʼ������ģ�飬UltrasonicWave_StartMeasure��������
			 ������࣬������õ�����ͨ������1��ӡ����         
 * ʵ��ƽ̨��Mini STM32������  STM32F103ZET6
 * Ӳ�����ӣ�------------------
 *          | PC2  - TRIG      |
 *          | PC3  - ECHO      |
 *           ------------------
 * ��汾  ��ST3.5.0
 *
 * ����    ��2617 
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
float Distance1=0;      //������ľ���   


/*
 * ��������UltrasonicWave_Configuration
 * ����  ��������ģ��ĳ�ʼ��
 * ����  ����
 * ���  ����	
 */
void UltrasonicWave_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;	
	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);	//�ر�jtag
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO, ENABLE);
    
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;					 //PC2��TRIG
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		     //��Ϊ�������ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	         
  GPIO_Init(GPIOC, &GPIO_InitStructure);	                 //��ʼ������GPIO 

  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;				     //PC3��ECH0
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;		 //��Ϊ����
  GPIO_Init(GPIOC,&GPIO_InitStructure);						 //��ʼ��GPIOA
	
	 //GPIOC.3	  �ж����Լ��жϳ�ʼ������
 	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource3);//----------------------P3

	EXTI_InitStructure.EXTI_Line=EXTI_Line3;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);		//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
		
			
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;			//ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =1;	//��ռ���ȼ�2 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;					//�����ȼ�2 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);  	  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
}


void EXTI3_IRQHandler(void)
{
	u8 i=0;
	float result=0;
	delay_us(10);		                      //��ʱ10us
  if(EXTI_GetITStatus(EXTI_Line3) != RESET)
	{		
		TIM_SetCounter(TIM2,0);
		TIM_Cmd(TIM2, ENABLE);                                     //����ʱ��
	
		while(GPIO_ReadInputDataBit(GPIOC,ECHO_PIN));	             //�ȴ��͵�ƽ
		TIM_Cmd(TIM2, DISABLE);			                               //��ʱ��2ʧ��
		
		Distance0	=TIM_GetCounter(TIM2)*5*34/100.0;									 //�������&&UltrasonicWave_Distance<150
		// 		if(Distance0>2&&Distance0<450)
// 		{
// 			printf("��1:%3.2f \n",Distance0);
//  			delay_ms(10);
// 		}
		
		EXTI_ClearITPendingBit(EXTI_Line3);  												//���EXTI7��·����λ
}

}
/*
 * ��������UltrasonicWave_StartMeasure
 * ����  ����ʼ��࣬����һ��>10us�����壬Ȼ��������صĸߵ�ƽʱ��
 * ����  ����
 * ���  ����	
 */
void UltrasonicWave_StartMeasure(void)
{
	TRIG_PIN=1; 		  					//��>10US�ĸߵ�ƽ�TRIG_PORT,TRIG_PIN
  delay_us(20);		                      //��ʱ20US
	TRIG_PIN=0;
	delay_ms(50);
}

/******************* (C) 1209 Lab *****END OF FILE************/

