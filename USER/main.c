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
int is_Mission_End = 0;//�����Ƿ���ɱ�־
int start_Node = 0;

void System_Init(void);
void Show_Init(void);
void StartAndGetData_HMC5883L(void);
void Steering_Control(int is_start, int pre_angle);
int get_Turn_Around_Angle(void);

int main(void){		
	
	System_Init();
	
	while(1){
		Indicator_Light_Process_Done = 1;  //����ִ��ָʾ��
		delay_ms(2000);
		
		/**********��ʼ·���滮*********************/	
		while(!is_path_prepare){				
			//·���滮��׼���ڵ����ݺͶ�Ӧ�ĽǶ�����
			if(pre_Node_Count ==0){	//�״ε���·���滮��������ʼ�ڵ�Ϊ0�ڵ�
				plan_Route(pre_Node_Count);
			}else{									//���״ε���·���滮��������ʼ�ڵ�Ϊact_Node_Count����һ����ʻ��󵽴�Ľڵ�����
				plan_Route(act_Node_Count);
				pre_Node_Count = 0;						//����һ��·���滮���ýڵ�������
				act_Node_Count = 0;						//ʵ�ʾ����Ľڵ���������
			}				
			combine_Angle_Data(10);						//��Ͻڵ��Ӧλ�ýǶ�����-----�����ͼ�������ı䣬�ǵ��޸Ĳ���������
			LCD5110ShowNum(30, 2, pre_Node_Count);	
			if(is_Turn_Around_Flag == 1){
				is_Turn_Around_Flag = 0; 					//	������ͷ����־����
				Steering_Control(1, get_Turn_Around_Angle());
			}
//				LED_TEST();		//---test
		}		
		
		/**********��ʼ�ڵ��� *********************/		
		while(!is_First_Node_Flag){		 //ֱ����⵽��ʼ�ڵ�����������  ִֻ��һ�δ�ѭ����is_First_Node_Flag��1��û�лָ���
						LED_TEST();		//---test
			Node_Detection();					
			if(is_First_Node_Flag==1 && act_Node_Count==0){												
				act_Node_Count = 1;	//��⵽�˳�ʼ�ڵ� act_Node_Count ��1	
				LCD5110ShowNum(70, 2, act_Node_Count);					
				is_Steering_Control_Begin = 1;	//��⵽��ʼ�ڵ���ת����Ʊ�־��1		
			}	
		} 
		
		/**********ת�����*********************/
		Steering_Control(is_Steering_Control_Begin, get_Angle_Value(act_Node_Count-1));	//��-1������Ϊ��һ���ڵ��Ӧ�Ƕ����������0��λ�õ�����
		
//									Steering_Control(1, 180);		//----TEST
		/**********ѭ����ͬʱ����ʱ��---�ڵ��⣩*********************/
		Tracking(is_steering_OK);
//										Tracking(1);//----test		

		Indicator_Light_Process_Done = 0;  //����ִ��ָʾ��
		delay_ms(5000);
		/***************************�������****************************/
		while(is_Mission_End){
			stop();
			Indicator_Light_Process_Done = !Indicator_Light_Process_Done;
			LCD5110ClearPart(0, 0, 100, 5);	//����������Ϊ�Ƕ���ʾ��׼��
			delay_ms(5000);
			LCD5110WriteEnStr(2, 2, (u8 *)"MISSION DONE!");
			delay_ms(5000);			
		}				
	}
}

void System_Init(void)
{
	delay_init();	    	 //��ʱ������ʼ��
	NVIC_Configuration(); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	Timerx_Init2(5000,7199);   //������sensorʹ�ö�ʱ����ʼ�� 10Khz�ļ���Ƶ�ʣ�������5000Ϊ500ms 
	TIM1_Int_Init(9,7199);//TIM3��ʱ����ʼ��---���ڽڵ���������
	TIM5_Int_Init(999, 7199); //TIM5��ʼ��
	uart_init(9600);  //���ڳ�ʼ�� ��ʼ�����ض˿ڵ�
	LED_Init();	//ϵͳ����ģ��ָʾ�� ���ų�ʼ��
	Moto_Init();
	INFRARED_Init(); //��·Ѱ���������϶˿ڳ�ʼ��
	InfraredSafeDistance_Init(); //���Ҳ������ϰ�ȫ����˿ڳ�ʼ��		
	Init_HMC5883L();	
	UltrasonicWave_Configuration();               //�Գ�����ģ���ʼ��
	LCD5110_GPIOInit();
	Show_Init();	//Nokia5110��ʼ��ʾ����
	
}
//Nokia5110��ʾ����ʼ��
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
//����HMC5883L���������
void StartAndGetData_HMC5883L(){
	short x,y,z;
	Multiple_Read_HMC5883L();	
	x=BUF[0] << 8 | BUF[1]; //Combine MSB and LSB of X Data output register
	z=BUF[2] << 8 | BUF[3]; //Combine MSB and LSB of Z Data output register
	y=BUF[4] << 8 | BUF[5]; //Combine MSB and LSB of Y Data output register
	angle= atan2((double)y,(double)x) * (180 / 3.14159265) + 180; // angle in degrees ˮƽ��
//	angle*=10;   //����ʮ��������ʾ(��ʾǧλ�൱��ʵ����ʾ��λ������)	
	
	//��ʾ5883L����	
	LCD5110ShowNum(10, 0, x);
	LCD5110ShowNum(10, 1, y);
	LCD5110ShowNum(35, 3, angle);	
}
/***
���ܣ�ת�����
������pre_angle---�ڵ��Ӧ��֧·�Ƕ�
****/
void Steering_Control(int is_start, int pre_angle){
	if(is_start){
		int temp_angle = 0;
		int is_turning = 1;
		
		is_steering_OK = 0;			//��0ת���־
		LCD5110ClearPart(30, 3, 70, 1);	//����������Ϊ�Ƕ���ʾ��׼��
		LCD5110ShowNum(60, 3, pre_angle);	//��ʾԤ��Ƕ�����
		while(is_turning){
			StartAndGetData_HMC5883L();
			//ȡת��ǵľ���ֵ
			if(pre_angle>angle){
				temp_angle = pre_angle-angle;
			}else if(pre_angle==angle){
				temp_angle = 0;
			}else{
				temp_angle = angle-pre_angle;
			}

			if(temp_angle>-2 && temp_angle<2){						//����ʵ������޸���ֵ
				delay_ms(500);	
				is_steering_OK = 1;//ת��ɹ�  ��־��1
				is_turning = 0;    //����ѭ��---ѭ��������־
				is_Steering_Control_Begin = 0;	//�ر�ת����Ʊ�־
				Indicator_Light_Node_Detection = 0; //�ڵ���ָʾ�ƹ�
//				is_Detect_Node = 0;			//�ڵ�����0
			}else if(temp_angle<=180){
				if(angle<pre_angle){
					turn_Right();//test---��ת  ˳ʱ��
				}else if(angle>pre_angle){
					turn_Left(); //test---��ת  ��ʱ��
				}				
			}else if(temp_angle>180 && temp_angle<=360){
				if(angle>pre_angle){
					turn_Right(); //test---��ת  ˳ʱ��
				}else if(angle<pre_angle){
					turn_Left(); //test---��ת  ��ʱ��
				}
			}
		}	
	}	
}
//��õ�ͷ�ĽǶ�
int get_Turn_Around_Angle(){
	int temp = 0;
	StartAndGetData_HMC5883L();	//��õ�ǰС����ͷ�Ƕ�---angle
	if(angle <= 180){
		temp = angle + 180;
	}else{
		temp = angle - 180;
	}
	return temp;
}

