#include "infrared.h"
#include "stm32f10x.h"
/**************************************************************************
函数功能：八路寻迹和红外避障
入口参数：
返回  值：
**************************************************************************/

void INFRARED_Init(void){
		/*GPIO端口配置结构体变量定义*/
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD, ENABLE);//打开GPIOA/GPIOD时钟	
	GPIO_InitStructure.GPIO_Pin	= GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;//设置输入端口引脚---PC6.7.8.9
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置为上拉输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4; //设置输入端口引脚---PD1.2.3.4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);	
}
