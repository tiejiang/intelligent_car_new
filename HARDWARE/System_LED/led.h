#ifndef _LED_H
#define _LED_H

#define Indicator_Light_Process_Done   			PFout(1)// PF1	经过一个节点、完成一个过程的指示灯
#define Indicator_Light_TIM1 								PFout(2)// PF2	TIM1定时器工作指示灯
#define Indicator_Light_Node_Detection			PFout(4)// PF4	节点检测指示灯
#define Indicator_Light_Tracking						PFout(5)// PF5	循迹指示灯
#define Indicator_Light_Obstacle_Detection	PFout(6)// PF6	障碍探测指示灯

void LED_Init(void);
void LED_TEST(void);
#endif
