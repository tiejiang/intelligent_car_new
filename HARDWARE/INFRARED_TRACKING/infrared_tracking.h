#ifndef __INFRARED_H
#define __INFRARED_H	
#define MIDDLE_L PCin(6)//车身中部左侧光电传感器
#define SENSORL3 PCin(7)//小车前方左侧起第三个光电传感器
#define SENSORL2 PCin(8)//小车前方左侧起第二个光电传感器
#define SENSORL1 PCin(9)//小车前方左侧起第一个光电传感器

#define MIDDLE_R PDin(4)//车身中部右侧光电传感器
#define SENSORR3 PDin(3)//小车前方右侧起第三个光电传感器
#define SENSORR2 PDin(2)//小车前方右侧起第二个光电传感器
#define SENSORR1 PDin(1)//小车前方右侧起第一个光电传感器

void INFRARED_Init(void);		//IO初始化
int Node_Detection(void);   //节点检测
void Tracking(int is_Tracking);         //循迹
#endif
extern int act_Node_Count;
extern int is_Tracking_run;
extern int is_First_Node_Flag;
extern int is_Detect_Node;
extern int is_Node_Detection_Start;
extern int test_Drive;
