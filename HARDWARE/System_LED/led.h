#ifndef _LED_H
#define _LED_H

#define Indicator_Light_Process_Done   			PFout(1)// PF1	����һ���ڵ㡢���һ�����̵�ָʾ��
#define Indicator_Light_TIM1 								PFout(2)// PF2	TIM1��ʱ������ָʾ��
#define Indicator_Light_Node_Detection			PFout(4)// PF4	�ڵ���ָʾ��
#define Indicator_Light_Tracking						PFout(5)// PF5	ѭ��ָʾ��
#define Indicator_Light_Obstacle_Detection	PFout(6)// PF6	�ϰ�̽��ָʾ��

void LED_Init(void);
void LED_TEST(void);
#endif
