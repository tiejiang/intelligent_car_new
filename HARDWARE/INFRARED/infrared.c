#include "infrared.h"
#include "stm32f10x.h"
/**************************************************************************
�������ܣ���·Ѱ���ͺ������
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
