#ifndef __INFRARED_H
#define __INFRARED_H	 
#define SENSORL1 PCin(6)//С��ǰ��������һ����紫����
#define SENSORL2 PCin(7)//С��ǰ�������ڶ�����紫����
#define SENSORL3 PCin(8)//С��ǰ��������������紫����
#define SENSORL4 PCin(9)//С��ǰ���������ĸ���紫����

#define SENSORR5 PDin(4)//С��ǰ�������������紫����
#define SENSORR6 PDin(3)//С��ǰ��������������紫����
#define SENSORR7 PDin(2)//С��ǰ���������߸���紫����
#define SENSORR8 PDin(1)//С��ǰ�������ڰ˸���紫����
void INFRARED_Init(void);		//IO��ʼ��
#endif
