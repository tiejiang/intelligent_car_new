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
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); //时钟使能
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
//	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE ); //使能指定的TIM5中断,允许更新中断
	TIM_ITConfig(TIM5,TIM_IT_Update,DISABLE); //!!!初始失能指定的TIM5中断,初始不允许更新中断（如果初始不允许中断执行，那么必须关掉这个）

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  //TIM5中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  //先占优先级3级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;  //从优先级4级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器

//	TIM_Cmd(TIM5, ENABLE);  //使能TIMx			
	TIM_Cmd(TIM5, DISABLE);  ////初始状态 失能 TIM3
}
//关定时器
void Disable_TIM5(){
	Indicator_Light_Obstacle_Detection = 0;   //关闭障碍探测指示灯	
	TIM_ITConfig(TIM5,TIM_IT_Update,DISABLE);
	TIM_Cmd(TIM5, DISABLE);	
}
//开定时器
void Enable_TIM5(){	
	Indicator_Light_Obstacle_Detection = 1;//障碍探测指示灯开
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM5, ENABLE);
}
//定时器5中断服务程序
int is_Turn_Around_Flag = 0;			//障碍物探测后的“调头”标志
void TIM5_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)  //检查TIM5更新中断发生与否
		{
		TIM_ClearITPendingBit(TIM5, TIM_IT_Update  );  //清除TIMx更新中断标志 		
		
		UltrasonicWave_StartMeasure();                //开始测距，发送一个>10us的脉冲，然后测量返回的高电平时间
		//显示距离数据
		LCD5110ShowNum(45,5,Distance0);
			if(Distance0 < SAFE_DISTANCE && Distance0>10){
				is_Tracking_run = 0;	//禁止循迹功能
				change_Map(act_Node_Count-1);  	//更新地图矩阵和角度矩阵
				
				is_path_prepare = 0;					//更新路径规划标志---重新进入主循环里的第一个循环			 	
				is_Turn_Around_Flag = 1;					//将“调头”标志置1
				
				
				Disable_TIM1();									//关闭TIM1节点检测
				Disable_TIM5();									//关闭TIM5定时器中断
			}
		}
}



