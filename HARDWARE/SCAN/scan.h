#ifndef __SCAN_H
#define __SCAN_H	 
#include "sys.h"

#define STOP	1		//KEY0����
#define INFRARED	2		//KEY1����
#define ULTRA	3		//WK_UP����

#define START  PAin(7)   	//PA7
#define LINE  PAin(8)	 	//PA8 
	 
void SCAN_Init(void);		//IO��ʼ��
void MODEL_Scan(void);		//����ɨ�躯��
#endif
