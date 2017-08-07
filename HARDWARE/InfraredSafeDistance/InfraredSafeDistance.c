#include "InfraredSafeDistance.h"
#include "stm32f10x.h"

/*************************
智能小车两侧红外避障安全距离

**********************************/
void InfraredSafeDistance_Init(void){
		/*GPIO端口配置结构体变量定义*/
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//打开GPIOA时钟	
	GPIO_InitStructure.GPIO_Pin	= GPIO_Pin_2|GPIO_Pin_3;//设置输入端口引脚---PA2.3
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置为上拉输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}


