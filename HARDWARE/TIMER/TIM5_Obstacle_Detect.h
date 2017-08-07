#ifndef __TIM5_OBSTACLE_DETECT_H
#define __TIM5_OBSTACLE_DETECT_H
#include "sys.h"
#define SAFE_DISTANCE 15//小车和障碍物的安全距离

void TIM5_Int_Init(u16 arr,u16 psc);
void Disable_TIM5(void);
void Enable_TIM5(void); 
#endif
extern int is_Turn_Around_Flag ;
