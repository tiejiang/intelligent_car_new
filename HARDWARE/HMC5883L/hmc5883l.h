#ifndef _HMC5883_H
#define _HMC5883_H
//#include "sys.h"

#define	SlaveAddress   0x3C	  //定义器件在IIC总线中的从地址

#define I2C_SCL       PEout(0)
#define I2C_SDA       PEout(1)
#define I2C_SDA_READ  PEin(1)
#define I2C_SDAin {GPIOE->CRL&=0xffffff0f;GPIOE->CRL|=0x00000080;}	      
#define I2C_SDAout {GPIOE->CRL&=0xffffff0f;GPIOE->CRL|=0x00000030;}

//#define I2C_SCL       PBout(0)
//#define I2C_SDA       PBout(1)
//#define I2C_SDA_READ  PBin(1)
//#define I2C_SDAin {GPIOB->CRL&=0xffffff0f;GPIOB->CRL|=0x00000080;}	      
//#define I2C_SDAout {GPIOB->CRL&=0xffffff0f;GPIOB->CRL|=0x00000030;}
void Init_HMC5883L(void);
void Multiple_Read_HMC5883L(void);
void StartAndGetData_HMC5883L();


#endif
extern char BufData[32];
//extern void Init_HMC5883L(void);
//extern void Multiple_Read_HMC5883L(void);
//extern void StartAndGetData_HMC5883L();
extern unsigned char BUF[8];
extern int is_Steering_Control_Begin;
extern int is_steering_OK;



