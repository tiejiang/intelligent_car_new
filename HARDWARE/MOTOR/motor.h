#ifndef __MOTOR_H
#define __MOTOR_H	 
#include "sys.h"
#include "stm32f10x.h"
#define Moto_PwmMax 999
#define MOTOR_SPEED 500
#define TURN_MOTOR_SPEED 750

void Left_Moto_Pwm(int16_t MOTO1_PWM,int16_t MOTO2_PWM,int16_t MOTO3_PWM,int16_t MOTO4_PWM);
void Right_Moto_Pwm(int16_t MOTO1_PWM,int16_t MOTO2_PWM,int16_t MOTO3_PWM,int16_t MOTO4_PWM);
void Moto_Init(void);
void Tim1_init(void);
void Servo_PWMSet(int16_t MOTO1_PWM);
void turn_Left(void);
void turn_Right(void);
void stop();
#endif

