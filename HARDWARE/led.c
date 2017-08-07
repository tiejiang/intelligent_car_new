#include "led.h"
#include "stm32f10x.h"

void LED_Init(void){
		/*GPIO端口配置结构体变量定义*/
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//打开GPIO时钟
	//将LED的4个引脚分别设置为推挽输出
	GPIO_InitStructure.GPIO_Pin	= GPIO_Pin_All;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}
void Delay(u32 t){
	for(; t>0; t--);
}
void LED_Set(void){

//	if(GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_6)==0){
//		GPIO_SetBits(GPIOF, GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9);
//		Delay(4000000);
//		GPIO_ResetBits(GPIOF, GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9);
//		Delay(4000000);
//	}
//	else{
//		GPIO_SetBits(GPIOF, GPIO_Pin_8 | GPIO_Pin_9);
//		Delay(4000000);
//		GPIO_ResetBits(GPIOF, GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9);
//		Delay(4000000);
//	}
		GPIO_SetBits(GPIOB, GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9);
		Delay(4000000);
		GPIO_ResetBits(GPIOB, GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9);
		Delay(4000000);
}


