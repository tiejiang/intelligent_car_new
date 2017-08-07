#include "TIM5_Obstacle_Detect.h"
#include "led.h"
#include "stm32f10x_tim.h"
#include "infrared_tracking.h"
#include <stdio.h>
#include "ultrasonic.h"
#include "LCD5110.h"
#include "Dynamic_Map.h"
#include "timer.h"

float temp;
//char DisNum[16];

void TIM5_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
//	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM5�ж�,��������ж�
	TIM_ITConfig(TIM5,TIM_IT_Update,DISABLE); //!!!��ʼʧ��ָ����TIM5�ж�,��ʼ����������жϣ������ʼ�������ж�ִ�У���ô����ص������

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  //TIM5�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  //��ռ���ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;  //�����ȼ�4��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���

//	TIM_Cmd(TIM5, ENABLE);  //ʹ��TIMx			
	TIM_Cmd(TIM5, DISABLE);  ////��ʼ״̬ ʧ�� TIM3
}
//�ض�ʱ��
void Disable_TIM5(){
	Indicator_Light_Obstacle_Detection = 0;   //�ر��ϰ�̽��ָʾ��	
	TIM_ITConfig(TIM5,TIM_IT_Update,DISABLE);
	TIM_Cmd(TIM5, DISABLE);	
}
//����ʱ��
void Enable_TIM5(){	
	Indicator_Light_Obstacle_Detection = 1;//�ϰ�̽��ָʾ�ƿ�
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM5, ENABLE);
}
//��ʱ��5�жϷ������
int is_Turn_Around_Flag = 0;			//�ϰ���̽���ġ���ͷ����־
void TIM5_IRQHandler(void)   //TIM3�ж�
{
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)  //���TIM5�����жϷ������
		{
		TIM_ClearITPendingBit(TIM5, TIM_IT_Update  );  //���TIMx�����жϱ�־ 		
		
		UltrasonicWave_StartMeasure();                //��ʼ��࣬����һ��>10us�����壬Ȼ��������صĸߵ�ƽʱ��
		//��ʾ��������
		LCD5110ShowNum(45,5,Distance0);
			if(Distance0 < SAFE_DISTANCE && Distance0>10){
				is_Tracking_run = 0;	//��ֹѭ������
				change_Map(act_Node_Count-1);  	//���µ�ͼ����ͽǶȾ���
				
				is_path_prepare = 0;					//����·���滮��־---���½�����ѭ����ĵ�һ��ѭ��			 	
				is_Turn_Around_Flag = 1;					//������ͷ����־��1
				
				
				Disable_TIM1();									//�ر�TIM1�ڵ���
				Disable_TIM5();									//�ر�TIM5��ʱ���ж�
			}
		}
}



