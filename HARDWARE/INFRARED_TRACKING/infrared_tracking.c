#include "infrared_tracking.h"
#include "stm32f10x.h"
#include "sys.h"
#include "motor.h"
#include "timer.h"
#include "led.h"
#include "TIM5_Obstacle_Detect.h"
#include "hmc5883l.h"
#include "delay.h"
#include "LCD5110.h"

/**************************************************************************
�������ܣ�Ѱ���İ˸��˿����Ŷ��� PC6/7/8/9�Լ�PD1/2/3/4
��ڲ�����
����  ֵ��
**************************************************************************/

void INFRARED_Init(void){
		/*GPIO�˿����ýṹ���������*/
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD, ENABLE);//��GPIOA/GPIODʱ��	
	GPIO_InitStructure.GPIO_Pin	= GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;//��������˿�����---PC6.7.8.9
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //����Ϊ��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4; //��������˿�����---PD1.2.3.4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);	
}

/**************************************************************************
��������: �ڵ��⣨������ʼ�ڵ��⹦�ܣ�
��ڲ�����SENSORL2,SENSORL3,SENSORL4,
					SENSORR5,SENSORR6,SENSORR7
����  ֵ��(bool)Is_Node_Detection_OK
**************************************************************************/
int act_Node_Count = 0; //��¼�ڵ���
int is_First_Node_Flag = 0; //�Ƿ�Ϊ��ʼ�ڵ� 1:��  0������
int is_Detect_Node = 0;		//����Ƿ��⵽�ڵ�  1����  0������
int is_Node_Detection_Start = 0;	//�ڵ����Ƿ�ʼ����
int Node_Detection(){	
	
	if((SENSORL2==1&&SENSORL3==1) || (SENSORR2==1&&SENSORR3==1)){
		is_Detect_Node = 1;	
		Indicator_Light_Node_Detection = 1; //�ڵ���ָʾ�ƿ�	
		if(act_Node_Count==0){
			is_First_Node_Flag = 1;		
		}
//		else{
//			is_First_Node_Flag = 0;
//		}
	}else{
		is_Detect_Node = 0;
		Indicator_Light_Node_Detection = 0; //�ڵ���ָʾ�ƹ�
		return 0;
	}
}


/**************************************************************************
��������: ѭ��
��ڲ�����SENSORL4,SENSORR5,
					SENSORL1,SENSORL3,SENSORR6,SENSORR8
����  ֵ��
**************************************************************************/
int is_Tracking_run = 0;
int test_Drive = 0;
void Tracking(int is_Tracking){	
	int INFRARED_ANGLE;
	int PWM_ANGLE=0;

	//���ݱ�־
	if(is_Tracking == 1){
		is_Tracking_run = 1;
	}else{
		is_Tracking_run = 0;
	}	
	is_Detect_Node = 0;			//��0�ڵ����־
	is_Steering_Control_Begin = 0;	//�ر�ת����Ʊ�־
	is_steering_OK = 0;//�ر�ת��ɹ���־	
	
	Enable_TIM1();					//�����ڵ���Ķ�ʱ��	
//			is_Node_Detection_Start =1;	
	Enable_TIM5();		//���ϰ�̽�ⶨʱ���ж�TIM5
	while(is_Tracking_run){	
		Indicator_Light_Tracking = 1;		//ѭ��ָʾ�ƿ�			
		if((SENSORL3==1&&SENSORL2==0) || (SENSORL2==1&&SENSORL3==0) || SENSORL1==1){
			Left_Moto_Pwm(0, MOTOR_SPEED, 0, MOTOR_SPEED);	//
			Right_Moto_Pwm(MOTOR_SPEED, 0, MOTOR_SPEED, 0);
//			delay_ms(50);
//			stop();
		}else if((SENSORR3==1&&SENSORR2==0) || (SENSORR2==1&&SENSORR3==0) || SENSORR1==1){			
			Left_Moto_Pwm(MOTOR_SPEED, 0, MOTOR_SPEED, 0);
			Right_Moto_Pwm(0, MOTOR_SPEED, 0, MOTOR_SPEED);	//
//			delay_ms(50);
//			stop();
		}else{
			Left_Moto_Pwm(MOTOR_SPEED, 0, MOTOR_SPEED, 0);
			Right_Moto_Pwm(MOTOR_SPEED, 0, MOTOR_SPEED, 0);	//	//������ʻ
		}
	}
	if(!is_Tracking_run){
		Indicator_Light_Tracking = 0;		//ѭ��ָʾ�ƹ�
		stop();
	}
	//�ڵ㴦��̽ǰ��---��������λ��
	while(test_Drive){
//							LED_TEST();		//---test
		if(MIDDLE_L==1 || MIDDLE_R==1){
			is_Steering_Control_Begin = 1;//ת����ƴ�	
			test_Drive = 0;
		}else{
			Left_Moto_Pwm(TURN_MOTOR_SPEED, 0, TURN_MOTOR_SPEED, 0);
			Right_Moto_Pwm(TURN_MOTOR_SPEED, 0, TURN_MOTOR_SPEED, 0);
			delay_ms(20);		
			stop();
			delay_ms(2000);
		}

	}
}





