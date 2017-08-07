#include "stm32f10x.h"
#include "led.h"
#include "infrared_tracking.h"
#include "InfraredSafeDistance.h"
#include "sys.h"
#include "timer.h"
#include "TIM5_Obstacle_Detect.h"
#include "delay.h"
#include "motor.h"
#include "hmc5883l.h"
#include <stdio.h>
#include <math.h>
#include "LCD5110.h"
#include "ultrasonic.h"
#include "usart.h"
#include "Dynamic_Map.h"

char PRE_NODE[6] = "P_N";
char Act_NODE[6] = "A_N";
char Angle_Name[6] = "ANGLE";
char Distance_Name[4] = "DIS";
double angle;
int is_Mission_End = 0;//任务是否完成标志
int start_Node = 0;

void System_Init(void);
void Show_Init(void);
void StartAndGetData_HMC5883L(void);
void Steering_Control(int is_start, int pre_angle);
int get_Turn_Around_Angle(void);

int main(void){		
	
	System_Init();
	
	while(1){
		Indicator_Light_Process_Done = 1;  //任务执行指示灯
		delay_ms(2000);
		
		/**********初始路径规划*********************/	
		while(!is_path_prepare){				
			//路径规划、准备节点数据和对应的角度数据
			if(pre_Node_Count ==0){	//首次调用路径规划函数，起始节点为0节点
				plan_Route(pre_Node_Count);
			}else{									//非首次调用路径规划函数，起始节点为act_Node_Count即上一次行驶最后到达的节点数据
				plan_Route(act_Node_Count);
				pre_Node_Count = 0;						//将上一次路径规划所得节点数清零
				act_Node_Count = 0;						//实际经过的节点数据清零
			}				
			combine_Angle_Data(10);						//组合节点对应位置角度数据-----如果地图顶点数改变，记得修改参数！！！
			LCD5110ShowNum(30, 2, pre_Node_Count);	
			if(is_Turn_Around_Flag == 1){
				is_Turn_Around_Flag = 0; 					//	将“调头”标志清零
				Steering_Control(1, get_Turn_Around_Angle());
			}
//				LED_TEST();		//---test
		}		
		
		/**********初始节点检测 *********************/		
		while(!is_First_Node_Flag){		 //直到检测到初始节点程序才往下走  只执行一次此循环（is_First_Node_Flag置1后没有恢复）
						LED_TEST();		//---test
			Node_Detection();					
			if(is_First_Node_Flag==1 && act_Node_Count==0){												
				act_Node_Count = 1;	//检测到了初始节点 act_Node_Count 置1	
				LCD5110ShowNum(70, 2, act_Node_Count);					
				is_Steering_Control_Begin = 1;	//检测到初始节点则转向控制标志置1		
			}	
		} 
		
		/**********转向控制*********************/
		Steering_Control(is_Steering_Control_Begin, get_Angle_Value(act_Node_Count-1));	//“-1”是因为第一个节点对应角度数组里面第0个位置的数据
		
//									Steering_Control(1, 180);		//----TEST
		/**********循迹（同时开定时器---节点检测）*********************/
		Tracking(is_steering_OK);
//										Tracking(1);//----test		

		Indicator_Light_Process_Done = 0;  //任务执行指示灯
		delay_ms(5000);
		/***************************任务完成****************************/
		while(is_Mission_End){
			stop();
			Indicator_Light_Process_Done = !Indicator_Light_Process_Done;
			LCD5110ClearPart(0, 0, 100, 5);	//部分清屏，为角度显示做准备
			delay_ms(5000);
			LCD5110WriteEnStr(2, 2, (u8 *)"MISSION DONE!");
			delay_ms(5000);			
		}				
	}
}

void System_Init(void)
{
	delay_init();	    	 //延时函数初始化
	NVIC_Configuration(); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	Timerx_Init2(5000,7199);   //超声波sensor使用定时器初始化 10Khz的计数频率，计数到5000为500ms 
	TIM1_Int_Init(9,7199);//TIM3定时器初始化---调节节点检测灵敏度
	TIM5_Int_Init(999, 7199); //TIM5初始化
	uart_init(9600);  //串口初始化 初始化下载端口等
	LED_Init();	//系统工作模块指示灯 引脚初始化
	Moto_Init();
	INFRARED_Init(); //四路寻迹与红外避障端口初始化
	InfraredSafeDistance_Init(); //左右侧红外避障安全距离端口初始化		
	Init_HMC5883L();	
	UltrasonicWave_Configuration();               //对超声波模块初始化
	LCD5110_GPIOInit();
	Show_Init();	//Nokia5110初始显示内容
	
}
//Nokia5110显示屏初始化
void Show_Init(void)
{
	LCD5110Clear();
	LCD5110WriteChar(0, 0,88);//"X"
	LCD5110WriteChar(0, 1,89);//"Y"
	LCD5110WriteEnStr(0, 2,(u8 *)PRE_NODE);
	LCD5110WriteEnStr(45, 2,(u8 *)Act_NODE);
	LCD5110WriteEnStr(0, 3,(u8 *)Angle_Name);//"ANGLE"
	LCD5110WriteEnStr(0, 5,(u8 *)Distance_Name);//"DIS"
}
//启动HMC5883L并获得数据
void StartAndGetData_HMC5883L(){
	short x,y,z;
	Multiple_Read_HMC5883L();	
	x=BUF[0] << 8 | BUF[1]; //Combine MSB and LSB of X Data output register
	z=BUF[2] << 8 | BUF[3]; //Combine MSB and LSB of Z Data output register
	y=BUF[4] << 8 | BUF[5]; //Combine MSB and LSB of Y Data output register
	angle= atan2((double)y,(double)x) * (180 / 3.14159265) + 180; // angle in degrees 水平面
//	angle*=10;   //扩大十倍便于显示(显示千位相当于实际显示百位的数据)	
	
	//显示5883L数据	
	LCD5110ShowNum(10, 0, x);
	LCD5110ShowNum(10, 1, y);
	LCD5110ShowNum(35, 3, angle);	
}
/***
功能：转向控制
参数：pre_angle---节点对应的支路角度
****/
void Steering_Control(int is_start, int pre_angle){
	if(is_start){
		int temp_angle = 0;
		int is_turning = 1;
		
		is_steering_OK = 0;			//置0转向标志
		LCD5110ClearPart(30, 3, 70, 1);	//部分清屏，为角度显示做准备
		LCD5110ShowNum(60, 3, pre_angle);	//显示预存角度数据
		while(is_turning){
			StartAndGetData_HMC5883L();
			//取转向角的绝对值
			if(pre_angle>angle){
				temp_angle = pre_angle-angle;
			}else if(pre_angle==angle){
				temp_angle = 0;
			}else{
				temp_angle = angle-pre_angle;
			}

			if(temp_angle>-2 && temp_angle<2){						//根据实际情况修改阈值
				delay_ms(500);	
				is_steering_OK = 1;//转向成功  标志置1
				is_turning = 0;    //跳出循环---循环结束标志
				is_Steering_Control_Begin = 0;	//关闭转向控制标志
				Indicator_Light_Node_Detection = 0; //节点检测指示灯关
//				is_Detect_Node = 0;			//节点检测置0
			}else if(temp_angle<=180){
				if(angle<pre_angle){
					turn_Right();//test---右转  顺时针
				}else if(angle>pre_angle){
					turn_Left(); //test---左转  逆时针
				}				
			}else if(temp_angle>180 && temp_angle<=360){
				if(angle>pre_angle){
					turn_Right(); //test---右转  顺时针
				}else if(angle<pre_angle){
					turn_Left(); //test---左转  逆时针
				}
			}
		}	
	}	
}
//获得调头的角度
int get_Turn_Around_Angle(){
	int temp = 0;
	StartAndGetData_HMC5883L();	//获得当前小车车头角度---angle
	if(angle <= 180){
		temp = angle + 180;
	}else{
		temp = angle - 180;
	}
	return temp;
}

