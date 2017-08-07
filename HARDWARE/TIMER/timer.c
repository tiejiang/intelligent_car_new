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
//定时器 驱动代码	  		  
//////////////////////////////////////////////////////////////////////////////////   	 

//通用定时器3中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
void TIM1_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); //时钟使能
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
//	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE ); //使能指定的TIM1中断,允许更新中断
	TIM_ITConfig(TIM1,TIM_IT_Update,DISABLE ); //初始不使能指定的TIM1中断,初始不允许更新中断（如果初始不允许中断执行，那么必须关掉这个）

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;  //TIM1中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器

//	TIM_Cmd(TIM1, ENABLE);  //使能TIMx		
	TIM_Cmd(TIM1, DISABLE);		//初始状态 失能 TIM1
}
//关定时器
void Disable_TIM1(){	
	Indicator_Light_TIM1 = 0;	// 定时器指示灯关
	TIM_ITConfig(TIM1,TIM_IT_Update,DISABLE);
	TIM_Cmd(TIM1, DISABLE);
}
//开定时器
void Enable_TIM1(){	
	Indicator_Light_TIM1 = 1;	// 定时器指示灯开
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM1, ENABLE);
}
//定时器3中断服务程序
extern int is_Mission_End;
int is_Node_Detection_TIM1_Start = 0; //是否开启节点检测定时器
void TIM1_UP_IRQHandler(void)   //TIM1中断	处理函数
{	
	if(TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET){  //检查TIM3更新中断发生与否			
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);  //清除TIMx更新中断标志 		
		
		Node_Detection();
		if(is_Detect_Node==1){
			act_Node_Count ++;
			LCD5110ShowNum(70, 2, act_Node_Count);	
			is_Tracking_run = 0;	//禁止循迹功能				
			if(act_Node_Count == pre_Node_Count){
				is_Mission_End = 1;			//实际节点数等于预先规划路径的节点数----任务完成标志置1
			}else{				
//				is_Steering_Control_Begin = 1;//转向控制打开									
					test_Drive = 1;		//监测到节点---开始调整车身位置以便开始转向
				is_Detect_Node = 0; 		//置0节点检测标志					
			}
			Disable_TIM5();				//关闭TIM5定时器中断
			Disable_TIM1();				//关闭定时器中断
		}			
			//左右侧红外避障安全距离处理
//				Safe_DistanceSensor();
	}
}

//通用定时器中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器2
void Timerx_Init2(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period =arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
  //TIM_ClearFlag(TIM2, TIM_FLAG_Update);							    		/* 清除溢出中断标志 */
	TIM_ITConfig(  //使能或者失能指定的TIM中断
		TIM2, //TIM2
		TIM_IT_Update  |  //TIM 中断源
		TIM_IT_Trigger,   //TIM 触发中断源 
		ENABLE  //使能
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

//	TIM_Cmd(TIM2, ENABLE);  //使能TIMx外设
							 
}
void TIM2_IRQHandler(void)   //TIM2中断
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
		{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //清除TIMx的中断待处理位:TIM 中断源 
			
		}
}



