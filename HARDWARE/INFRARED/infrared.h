#ifndef __INFRARED_H
#define __INFRARED_H	 
#define SENSORL1 PCin(6)//小车前方左侧起第一个光电传感器
#define SENSORL2 PCin(7)//小车前方左侧起第二个光电传感器
#define SENSORL3 PCin(8)//小车前方左侧起第三个光电传感器
#define SENSORL4 PCin(9)//小车前方左侧起第四个光电传感器

#define SENSORR5 PDin(4)//小车前方左侧起第五个光电传感器
#define SENSORR6 PDin(3)//小车前方左侧起第六个光电传感器
#define SENSORR7 PDin(2)//小车前方左侧起第七个光电传感器
#define SENSORR8 PDin(1)//小车前方左侧起第八个光电传感器
void INFRARED_Init(void);		//IO初始化
#endif
