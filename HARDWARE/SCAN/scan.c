#include "scan.h"
 	    
//������ʼ������ 
//PA0.15��PC5 ���ó�����
void SCAN_Init(void)
{
	RCC->APB2ENR|=1<<2;     //ʹ��PORTAʱ��
	//RCC->APB2ENR|=1<<4;     //ʹ��PORTCʱ��
	//JTAG_Set(SWD_ENABLE);	//�ر�JTAG,����SWD
	GPIOA->CRL&=0X0FFFFFFF;	//PA7���ó�����	  
	GPIOA->CRL|=0X80000000;   
	GPIOA->CRH&=0XFFFFFFF0;	//PA8���ó�����	  
	GPIOA->CRH|=0X00000008; 			 
	GPIOA->ODR&=0XFFFFFE7F;	   	//PA15����,PA0Ĭ������
} 
//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//����ֵ��
//0��û���κΰ�������
//KEY0_PRES��KEY0����
//KEY1_PRES��KEY1����
//WKUP_PRES��WK_UP���� 
//ע��˺�������Ӧ���ȼ�,KEY0>KEY1>WK_UP!!
int MODE=STOP;
void MODEL_Scan()
{
	if(START)  //START----PA7--�ߵ�ƽ���ʹ��
	{
		if(LINE) //PA8 ---�ߵ�ƽ����ѭ��ģʽ
			MODE=INFRARED;
		else      //PA8---�͵�ƽ��������ģʽ
			MODE=ULTRA;
	}else
	MODE=STOP;
}




















