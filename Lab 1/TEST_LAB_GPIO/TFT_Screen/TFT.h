#ifndef _TFT_H_
#define _TFT_H_
//
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
//
#include <string.h>
#include <stdlib.h>
//
//Basic Colors
#define CYAN	  	0x07FF
#define MAGNETA   	0xF81F
#define RED			0xf800
#define GREEN		0x07e0
#define BLUE		0x001f
#define BLACK		0x0000
#define YELLOW		0xffe0
#define WHITE		0xffff
//
#define BROWN		0XBC40
#define BRRED		0XFC07
#define GRAY		0X8430
#define DGRAY		0X2104	// 00100 001000 00100
#define DARKBLUE	0X01CF
#define LIGHTBLUE	0X7D7C
#define GRAYBLUE	0X5458
#define LIGHTGREEN	0X841F
#define LGRAY		0XC618
#define LGRAYBLUE	0XA651
#define LBBLUE		0X2B12
//
#define GRID			0x01E7
#define LGRID			0X430C	// 01000 011000 01100
#define BACKGROUND_32	0x2104
//
void TFT_Init(uint8_t orient);
//
void TFT_Draw_Pixel(uint16_t x, uint16_t y,uint16_t color);
//
void TFT_Fill_Screen(uint16_t x_left, uint16_t x_right, uint16_t y_up, uint16_t y_down, uint16_t color);
//
void TFT_BITMAP(uint16_t x, uint16_t y, uint16_t size_x, uint16_t size_y, uint16_t *bitmap);
//
void TFT_Send_Cmd(uint8_t cmd);
void TFT_Write_Data(uint16_t data);
void TFT_SetOrientation(uint8_t orient);
void TFT_Send_Data(uint16_t data);
void TFT_Set_Column(uint16_t start_column,uint16_t end_colunm);
void TFT_Set_Page(uint16_t start_page,uint16_t end_page);

uint32_t TFT_ReadID(void);
//
#endif 

