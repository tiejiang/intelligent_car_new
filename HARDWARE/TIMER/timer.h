#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//��ʱ�� ��������	  							  
//////////////////////////////////////////////////////////////////////////////////   

void Timerx_Init2(u16 arr,u16 psc);
void TIM1_Int_Init(u16 arr,u16 psc);
void Disable_TIM1(void);
void Enable_TIM1(void);
#endif
extern int is_Node_Detection_TIM1_Start; //�Ƿ����ڵ��ⶨʱ��
