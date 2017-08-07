#ifndef _LCD5110_H
#define _LCD5110_H
#include "sys.h"

//�������
#define LCD_X_RES		84
#define LCD_Y_RES		48

//ö�� D/Cģʽѡ�� 
typedef enum
{
  DC_CMD  = 0,	//д����
	DC_DATA = 1		//д����		
} DCType;

//------------------------------------------------------------��ֲ�޸���-----------------------------------------------------------------------

//#define LCD_CTRL_PORT	GPIOB
//#define LCDRCC_CLK  RCC_APB2Periph_GPIOB
//#define LCD_RST			GPIO_Pin_11
//#define LCD_CE			GPIO_Pin_12
//#define LCD_DC			GPIO_Pin_13
//#define LCD_MOSI		GPIO_Pin_14
//#define LCD_CLK			GPIO_Pin_15
// #define LCD_VCC			GPIO_Pin_
// #define LCD_BGL			GPIO_Pin_

#define LCD_CTRL_PORT	GPIOE
#define LCDRCC_CLK  RCC_APB2Periph_GPIOE
#define LCD_RST			GPIO_Pin_10
#define LCD_CE			GPIO_Pin_9
#define LCD_DC			GPIO_Pin_12
#define LCD_MOSI		GPIO_Pin_11
#define LCD_CLK			GPIO_Pin_13
//---------------------------------------------------------------------------------------------------------------------------------------------


#define LCD_RST_H		GPIO_SetBits(LCD_CTRL_PORT, LCD_RST)
#define LCD_RST_L		GPIO_ResetBits(LCD_CTRL_PORT, LCD_RST)

#define LCD_CE_H		GPIO_SetBits(LCD_CTRL_PORT, LCD_CE)
#define LCD_CE_L		GPIO_ResetBits(LCD_CTRL_PORT, LCD_CE)

#define LCD_DC_DATA		GPIO_SetBits(LCD_CTRL_PORT, LCD_DC)
#define LCD_DC_CMD		GPIO_ResetBits(LCD_CTRL_PORT, LCD_DC)

#define LCD_MOSI_H		GPIO_SetBits(LCD_CTRL_PORT, LCD_MOSI)
#define LCD_MOSI_L		GPIO_ResetBits(LCD_CTRL_PORT, LCD_MOSI)

#define LCD_CLK_H		GPIO_SetBits(LCD_CTRL_PORT, LCD_CLK)
#define LCD_CLK_L		GPIO_ResetBits(LCD_CTRL_PORT, LCD_CLK)

// #define LCD_VCC_H		GPIO_SetBits(LCD_CTRL_PORT, LCD_VCC)

// #define	LCD_BGL_H		GPIO_SetBits(LCD_CTRL_PORT, LCD_BGL)

// #define LCD_GND_L		GPIO_ResetBits(LCD_CTRL_PORT, LCD_GND)


void LCD5110_GPIOInit(void);
void LCD5110_Init(void)	;
void LCD5110_SetContrast(u8 contrast);
void LCD5110_SetXY(u8 X, u8 Y);
void LCD5110Clear(void);
void LCD5110ClearPart(u8 X,u8 Y,u8 L,u8 H);
void LCD5110_Send(u8 data, DCType dc);
void LCD5110WriteChar(u8 X, u8 Y,u8 ascii);
void LCD5110WriteEnStr(u8 X, u8 Y, u8* s);
u32 mypow(u8 m,u8 n);
void LCD5110ShowNum(u8 X,u16 Y,u32 num);
void LCD5110WriteChStr(u8 X, u8 Y,u8 n);

#endif

