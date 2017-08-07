#ifndef __SCAN_H
#define __SCAN_H	 
#include "sys.h"

#define STOP	1		//KEY0按下
#define INFRARED	2		//KEY1按下
#define ULTRA	3		//WK_UP按下

#define START  PAin(7)   	//PA7
#define LINE  PAin(8)	 	//PA8 
	 
void SCAN_Init(void);		//IO初始化
void MODEL_Scan(void);		//按键扫描函数
#endif
