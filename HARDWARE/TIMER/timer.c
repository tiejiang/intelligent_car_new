#include "timer.h"
#include "sys.h"
#include "infrared_tracking.h"
#include "stm32f10x_tim.h"
#include "motor.h"
#include "InfraredSafeDistance.h"
#include "led.h"
#include "hmc5883l.h"
#include "Dynamic_Map.h"
#include "TIM5_Obstacle_Detect.h"
#include "LCD5110.h"

int PWM_ANGLE=0;
int NORMAL_RUNNING = 1;
extern int PWM_SPEED; 

//////////////////////////////////////////////////////////////////////////////////	 
//��ʱ�� ��������	  		  
//////////////////////////////////////////////////////////////////////////////////   	 

//ͨ�ö�ʱ��3�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!
void TIM1_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
//	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM1�ж�,��������ж�
	TIM_ITConfig(TIM1,TIM_IT_Update,DISABLE ); //��ʼ��ʹ��ָ����TIM1�ж�,��ʼ����������жϣ������ʼ�������ж�ִ�У���ô����ص������

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;  //TIM1�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���

//	TIM_Cmd(TIM1, ENABLE);  //ʹ��TIMx		
	TIM_Cmd(TIM1, DISABLE);		//��ʼ״̬ ʧ�� TIM1
}
//�ض�ʱ��
void Disable_TIM1(){	
	Indicator_Light_TIM1 = 0;	// ��ʱ��ָʾ�ƹ�
	TIM_ITConfig(TIM1,TIM_IT_Update,DISABLE);
	TIM_Cmd(TIM1, DISABLE);
}
//����ʱ��
void Enable_TIM1(){	
	Indicator_Light_TIM1 = 1;	// ��ʱ��ָʾ�ƿ�
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM1, ENABLE);
}
//��ʱ��3�жϷ������
extern int is_Mission_End;
int is_Node_Detection_TIM1_Start = 0; //�Ƿ����ڵ��ⶨʱ��
void TIM1_UP_IRQHandler(void)   //TIM1�ж�	������
{	
	if(TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET){  //���TIM3�����жϷ������			
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);  //���TIMx�����жϱ�־ 		
		
		Node_Detection();
		if(is_Detect_Node==1){
			act_Node_Count ++;
			LCD5110ShowNum(70, 2, act_Node_Count);	
			is_Tracking_run = 0;	//��ֹѭ������				
			if(act_Node_Count == pre_Node_Count){
				is_Mission_End = 1;			//ʵ�ʽڵ�������Ԥ�ȹ滮·���Ľڵ���----������ɱ�־��1
			}else{				
//				is_Steering_Control_Begin = 1;//ת����ƴ�									
					test_Drive = 1;		//��⵽�ڵ�---��ʼ��������λ���Ա㿪ʼת��
				is_Detect_Node = 0; 		//��0�ڵ����־					
			}
			Disable_TIM5();				//�ر�TIM5��ʱ���ж�
			Disable_TIM1();				//�رն�ʱ���ж�
		}			
			//���Ҳ������ϰ�ȫ���봦��
//				Safe_DistanceSensor();
	}
}

//ͨ�ö�ʱ���жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��2
void Timerx_Init2(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE); //ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period =arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
  //TIM_ClearFlag(TIM2, TIM_FLAG_Update);							    		/* �������жϱ�־ */
	TIM_ITConfig(  //ʹ�ܻ���ʧ��ָ����TIM�ж�
		TIM2, //TIM2
		TIM_IT_Update  |  //TIM �ж�Դ
		TIM_IT_Trigger,   //TIM �����ж�Դ 
		ENABLE  //ʹ��
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

//	TIM_Cmd(TIM2, ENABLE);  //ʹ��TIMx����
							 
}
void TIM2_IRQHandler(void)   //TIM2�ж�
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
		{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //���TIMx���жϴ�����λ:TIM �ж�Դ 
			
		}
}



