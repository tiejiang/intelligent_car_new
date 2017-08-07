#include "led.h"
#include "stm32f10x.h"
#include "delay.h"

void LED_Init(void){
		/*GPIO�˿����ýṹ���������*/
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);//��GPIOʱ��
	//��LED��4�����ŷֱ�����Ϊ�������
	GPIO_InitStructure.GPIO_Pin	= GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOF, GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6);
}

//test code 
void LED_TEST(void){

		GPIO_SetBits(GPIOF, GPIO_Pin_3);
		delay_ms(1000);
		GPIO_ResetBits(GPIOF, GPIO_Pin_3);
		delay_ms(1000);
}


