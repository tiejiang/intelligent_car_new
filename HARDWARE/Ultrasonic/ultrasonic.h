#ifndef _ultrasonic_H
#define _ultrasonic_H

extern float Distance0,Distance1;  

#define	TRIG_PIN       PCout(2)  //TRIG       
#define	ECHO_PIN       PCin(3)	 //ECHO

void UltrasonicWave_Configuration(void);               //�Գ�����ģ���ʼ��
void UltrasonicWave_StartMeasure(void);                //��ʼ��࣬����һ��>10us�����壬Ȼ��������صĸߵ�ƽʱ��

#endif /* __UltrasonicWave_H */
extern float Distance0;

