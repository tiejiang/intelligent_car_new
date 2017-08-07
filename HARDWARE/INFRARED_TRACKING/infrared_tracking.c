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
函数功能：寻迹的八个端口引脚定义 PC6/7/8/9以及PD1/2/3/4
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

/**************************************************************************
函数功能: 节点检测（包括初始节点检测功能）
入口参数：SENSORL2,SENSORL3,SENSORL4,
					SENSORR5,SENSORR6,SENSORR7
返回  值：(bool)Is_Node_Detection_OK
**************************************************************************/
int act_Node_Count = 0; //记录节点数
int is_First_Node_Flag = 0; //是否为初始节点 1:是  0：不是
int is_Detect_Node = 0;		//标记是否检测到节点  1：是  0：不是
int is_Node_Detection_Start = 0;	//节点检测是否开始运行
int Node_Detection(){	
	
	if((SENSORL2==1&&SENSORL3==1) || (SENSORR2==1&&SENSORR3==1)){
		is_Detect_Node = 1;	
		Indicator_Light_Node_Detection = 1; //节点检测指示灯开	
		if(act_Node_Count==0){
			is_First_Node_Flag = 1;		
		}
//		else{
//			is_First_Node_Flag = 0;
//		}
	}else{
		is_Detect_Node = 0;
		Indicator_Light_Node_Detection = 0; //节点检测指示灯关
		return 0;
	}
}


/**************************************************************************
函数功能: 循迹
入口参数：SENSORL4,SENSORR5,
					SENSORL1,SENSORL3,SENSORR6,SENSORR8
返回  值：
**************************************************************************/
int is_Tracking_run = 0;
int test_Drive = 0;
void Tracking(int is_Tracking){	
	int INFRARED_ANGLE;
	int PWM_ANGLE=0;

	//传递标志
	if(is_Tracking == 1){
		is_Tracking_run = 1;
	}else{
		is_Tracking_run = 0;
	}	
	is_Detect_Node = 0;			//置0节点检测标志
	is_Steering_Control_Begin = 0;	//关闭转向控制标志
	is_steering_OK = 0;//关闭转向成功标志	
	
	Enable_TIM1();					//开启节点检测的定时器	
//			is_Node_Detection_Start =1;	
	Enable_TIM5();		//打开障碍探测定时器中断TIM5
	while(is_Tracking_run){	
		Indicator_Light_Tracking = 1;		//循迹指示灯开			
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
			Right_Moto_Pwm(MOTOR_SPEED, 0, MOTOR_SPEED, 0);	//	//正常行驶
		}
	}
	if(!is_Tracking_run){
		Indicator_Light_Tracking = 0;		//循迹指示灯关
		stop();
	}
	//节点处试探前进---调整车身位置
	while(test_Drive){
//							LED_TEST();		//---test
		if(MIDDLE_L==1 || MIDDLE_R==1){
			is_Steering_Control_Begin = 1;//转向控制打开	
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





