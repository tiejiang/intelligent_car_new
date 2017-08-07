#ifndef __INFRARED_H
#define __INFRARED_H	
#define MIDDLE_L PCin(6)//�����в�����紫����
#define SENSORL3 PCin(7)//С��ǰ��������������紫����
#define SENSORL2 PCin(8)//С��ǰ�������ڶ�����紫����
#define SENSORL1 PCin(9)//С��ǰ��������һ����紫����

#define MIDDLE_R PDin(4)//�����в��Ҳ��紫����
#define SENSORR3 PDin(3)//С��ǰ���Ҳ����������紫����
#define SENSORR2 PDin(2)//С��ǰ���Ҳ���ڶ�����紫����
#define SENSORR1 PDin(1)//С��ǰ���Ҳ����һ����紫����

void INFRARED_Init(void);		//IO��ʼ��
int Node_Detection(void);   //�ڵ���
void Tracking(int is_Tracking);         //ѭ��
#endif
extern int act_Node_Count;
extern int is_Tracking_run;
extern int is_First_Node_Flag;
extern int is_Detect_Node;
extern int is_Node_Detection_Start;
extern int test_Drive;
