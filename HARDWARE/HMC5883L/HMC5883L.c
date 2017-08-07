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
//��������HMC5883�ڲ��Ƕ����ݣ���ַ��Χ0x3~0x5
//
//******************************************************
u8 BUF[8];
void Multiple_Read_HMC5883L(void)
{   
	  u8 i;
    I2C_Start();                          //��ʼ�ź�
    I2C_SendByte(0x3C);           //�����豸��ַ+д�ź�
    I2C_SendByte(0x03);                   //���ʹ洢��Ԫ��ַ����0x3��ʼ	
    I2C_Start();                          //��ʼ�ź�
    I2C_SendByte(0x3D);         //�����豸��ַ+���ź�
	  for (i=0; i<6; i++)                      //������ȡ6����ַ���ݣ��洢��BUF
    {
        BUF[i] = I2C_RecvByte();          //BUF[0]�洢����
        if (i == 5)
        {
           I2C_SendACK(1);                //���һ��������Ҫ��NOACK
        }
        else
        {
          I2C_SendACK(0);                 //��ӦACK
        }
    }
    I2C_Stop();                           //ֹͣ�ź�
    delay_ms(5);
}
//��ʼ��HMC5883��������Ҫ��ο�pdf�����޸�****  
void Init_HMC5883L()
{
	 RCC->APB2ENR|=1<<6;    	//ʹ��PORTEʱ��	
	 GPIOE->CRL&=0xffffff00; //CRL�����˿ڵĵͰ�λ������ΪPE0/PE1 --- �������λԭ��������
	 GPIOE->CRL|=0x00000077; //PE0 PE1��©��� 50MHZ
	 GPIOE->ODR|=3;  //0x11---0x0011 ����0λ ��1λ��1---������ߵ�ƽ
	
		//�޸�ΪPB0 PB1�˿ڵļĴ����汾
//	 RCC->APB2ENR|=1<<3;    	//ʹ��PORTBʱ��	
//	 GPIOB->CRL&=0xffffff00; //CRL�����˿ڵĵͰ�λ������ΪPB0/PB1 --- �������λԭ��������
//	 GPIOB->CRL|=0x00000077; //PB0 PB1��©��� 50MHZ
//	 GPIOB->ODR|=3;  //0x11---0x0011 ����0λ ��1λ��1---������ߵ�ƽ
	
	//�޸�ΪPB8 PB9�˿ڵĿ⺯���汾
//	GPIO_InitTypeDef GPIO_InitStructure; 
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
//	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_8 | GPIO_Pin_9 ;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //��©���
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
   Single_WriteI2C(0x02,0x00);	
}

