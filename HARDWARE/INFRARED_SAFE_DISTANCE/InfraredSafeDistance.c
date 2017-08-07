#include "InfraredSafeDistance.h"
#include "stm32f10x.h"

/*************************
����С�����������ϰ�ȫ����˿����Ŷ��� PA2(���)PA3���Ҳࣩ

**********************************/
void InfraredSafeDistance_Init(void){
		/*GPIO�˿����ýṹ���������*/
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//��GPIOAʱ��	
	GPIO_InitStructure.GPIO_Pin	= GPIO_Pin_2|GPIO_Pin_3;//��������˿�����---PA2.3
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //����Ϊ��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}
//���Ҳ������ϰ�ȫ���봦��
void Safe_DistanceSensor(){
		if(!LeftSafeDistanceSensor){
			Left_Moto_Pwm(TURN_MOTOR_SPEED, 0, TURN_MOTOR_SPEED, 0);
			Right_Moto_Pwm(0, TURN_MOTOR_SPEED, 0, TURN_MOTOR_SPEED);	
		}
		else if(!RightSafeDistanceSensor){
			Left_Moto_Pwm(0, TURN_MOTOR_SPEED, 0, TURN_MOTOR_SPEED);
			Right_Moto_Pwm(TURN_MOTOR_SPEED, 0, TURN_MOTOR_SPEED, 0);
		}
}


