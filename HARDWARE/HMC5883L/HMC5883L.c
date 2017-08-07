#include "delay.h"
#include "hmc5883l.h"
#include "LCD5110.h"
#include <math.h>

int is_Steering_Control_Begin = 0;
int is_steering_OK = 0;
//**************************************
//I2Cstart
//**************************************
void I2C_Start()
{
    I2C_SDA=1;
    I2C_SCL=1;                   
    delay_us(2);                
    I2C_SDA=0;                   
    delay_us(2);                 
    I2C_SCL=0;                  
}
//**************************************
//I2Cstop
//**************************************
void I2C_Stop()
{
    I2C_SDA=0;                    
    I2C_SCL=1;                    
    delay_us(2);                 
    I2C_SDA=1;                    
    delay_us(2);                
}
//**************************************
//I2C transmit
//ack (0:ACK 1:NAK)
//**************************************
void I2C_SendACK(u8 ack)
{
    I2C_SDA=ack;                 
    I2C_SCL=1;                    
    delay_us(2);                 
    I2C_SCL=0;                    
    delay_us(2);                
}
//**************************************
//I2C received signal
//**************************************

u8 I2C_RecvACK()
{
	  u8 CY;
	  I2C_SCL = 1;
	  delay_us(2);
      I2C_SDAin;
      delay_us(2);
      CY = I2C_SDA_READ;
	  delay_us(2);
      I2C_SDAout;
      I2C_SCL=0;                
      delay_us(2);
      return CY;
}

//**************************************
//I2C transmit a byte data
//**************************************
u8 I2C_SendByte(u8 dat)
{
    u8 i;  
    for (i=0; i<8; i++)         
    {
			I2C_SDA =((dat<<i)&0x80)>>7; 
		  delay_us(2); 				 
		  I2C_SCL=1;                
		  delay_us(2);                          
		  I2C_SCL=0;             
    }
    return I2C_RecvACK();
}

//**************************************
// I2C received a byte data
//**************************************
u8 I2C_RecvByte()
{
    u8 i;
    u8 dat = 0;
    I2C_SDA=1; 
    delay_us(2);
	  I2C_SDAin;
    for (i=0; i<8; i++)         
    {
			  delay_us(2);
        dat <<= 1;
        I2C_SCL=1;                
        delay_us(2); 			
        dat |= I2C_SDA_READ;                          
        I2C_SCL=0;               
        delay_us(2);          
    }
		I2C_SDAout;
    return dat;
}
//**************************************
//I2C write a byte data
//**************************************
void Single_WriteI2C(u8 REG_Address,u8 REG_data)
{
    I2C_Start();                
    I2C_SendByte(SlaveAddress);   
    I2C_SendByte(REG_Address);   
    I2C_SendByte(REG_data);       
    I2C_Stop();                  
}
//**************************************
//I2C read a byte
//**************************************
u8 Single_ReadI2C(u8 REG_Address)
{
		u8 REG_data;
		I2C_Start();                   
		I2C_SendByte(SlaveAddress);    
		I2C_SendByte(REG_Address);     
		I2C_Start();                   
		I2C_SendByte(SlaveAddress+1); 
		REG_data=I2C_RecvByte();       
		I2C_SendACK(1);                
		I2C_Stop();                   
		return REG_data;
}

//******************************************************
//
//连续读出HMC5883内部角度数据，地址范围0x3~0x5
//
//******************************************************
u8 BUF[8];
void Multiple_Read_HMC5883L(void)
{   
	  u8 i;
    I2C_Start();                          //起始信号
    I2C_SendByte(0x3C);           //发送设备地址+写信号
    I2C_SendByte(0x03);                   //发送存储单元地址，从0x3开始	
    I2C_Start();                          //起始信号
    I2C_SendByte(0x3D);         //发送设备地址+读信号
	  for (i=0; i<6; i++)                      //连续读取6个地址数据，存储中BUF
    {
        BUF[i] = I2C_RecvByte();          //BUF[0]存储数据
        if (i == 5)
        {
           I2C_SendACK(1);                //最后一个数据需要回NOACK
        }
        else
        {
          I2C_SendACK(0);                 //回应ACK
        }
    }
    I2C_Stop();                           //停止信号
    delay_ms(5);
}
//初始化HMC5883，根据需要请参考pdf进行修改****  
void Init_HMC5883L()
{
	 RCC->APB2ENR|=1<<6;    	//使能PORTE时钟	
	 GPIOE->CRL&=0xffffff00; //CRL操作端口的低八位，具体为PE0/PE1 --- 清除这两位原来的设置
	 GPIOE->CRL|=0x00000077; //PE0 PE1开漏输出 50MHZ
	 GPIOE->ODR|=3;  //0x11---0x0011 将第0位 第1位置1---》输出高电平
	
		//修改为PB0 PB1端口的寄存器版本
//	 RCC->APB2ENR|=1<<3;    	//使能PORTB时钟	
//	 GPIOB->CRL&=0xffffff00; //CRL操作端口的低八位，具体为PB0/PB1 --- 清除这两位原来的设置
//	 GPIOB->CRL|=0x00000077; //PB0 PB1开漏输出 50MHZ
//	 GPIOB->ODR|=3;  //0x11---0x0011 将第0位 第1位置1---》输出高电平
	
	//修改为PB8 PB9端口的库函数版本
//	GPIO_InitTypeDef GPIO_InitStructure; 
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
//	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_8 | GPIO_Pin_9 ;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //开漏输出
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
   Single_WriteI2C(0x02,0x00);	
}

