#include "TFT.h"
#include "stm32f4xx_fsmc.h"
//
#define LCD_RAM   *(vu16*)((u32)0x60020000)  //disp Data ADDR
#define LCD_REG   *(vu16*)((u32)0x60000000)	 //disp Reg  ADDR
//
/*****************************************
PD14 -----FSMC_D0  ----D0
PD15 -----FSMC_D1  ----D1
PD0   -----FSMC_D2  ----D2
PD1   -----FSMC_D3  ----D3
PE7    -----FSMC_D4  ---D4
PE8    -----FSMC_D5  ---D5
PE9    -----FSMC_D6  ---D6
PE10  -----FSMC_D7   ----D7
PE11  -----FSMC_D8   ----D8
PE12  -----FSMC_D9   ----D9
PE13  -----FSMC_D10   ----D10
PE14  -----FSMC_D11   ----D11
PE15  -----FSMC_D12   ----D12
PD8   -----FSMC_D13   ----D13
PD9   -----FSMC_D14   ----D14
PD10 -----FSMC_D15   ----D15
PD4   -----FSMC_NOE -----RD
PD5   -----FSMC_NWE ----WR
PD7    -----FSMC_NE1  ----CS
PD11 -----FSMC_A16   ----RS
PB5-------------------LCD_RST
***************************************************************/
//
static void GPIO_AF_FSMC_Config(void)
{
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource0,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource1,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource4,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource7,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource8,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource9,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource10,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource11,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource14,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource15,GPIO_AF_FSMC);

	GPIO_PinAFConfig(GPIOE,GPIO_PinSource7,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource8,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource9,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource10,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource11,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource12,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource13,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource14,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource15,GPIO_AF_FSMC);
}
//
static void LCD_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE , ENABLE);
	//
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB, GPIO_Pin_5);
	//
	GPIO_AF_FSMC_Config();
	//FSMC-D0~D15: PD 14 15 0 1,PE 7 8 9 10 11 12 13 14 15,PD 8 9 10
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_8 | GPIO_Pin_9 |
			                      GPIO_Pin_10 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	//
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 |
			                      GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 |
			                      GPIO_Pin_15;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	/*
	 * PD4-FSMC_NOE  :LCD-RD
	 * PD5-FSMC_NWE  :LCD-WR
	 * PD7-FSMC_NE1  :LCD-CS
	 * PD11-FSMC_A16 :LCD-DC
	 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 ;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	//
	GPIO_SetBits(GPIOD, GPIO_Pin_13);
	GPIO_SetBits(GPIOD, GPIO_Pin_4);
	GPIO_SetBits(GPIOD, GPIO_Pin_5);
	GPIO_SetBits(GPIOD, GPIO_Pin_7);
}
//
static void LCD_FSMC_Config(void)
{
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
	FSMC_NORSRAMTimingInitTypeDef  p;
	LCD_GPIO_Config();
	RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC, ENABLE);
	p.FSMC_AddressSetupTime = 5; // 5
	p.FSMC_AddressHoldTime = 2;  //2
	p.FSMC_DataSetupTime = 9;   //9
	p.FSMC_BusTurnAroundDuration = 0;
	p.FSMC_CLKDivision = 0;
	p.FSMC_DataLatency = 0;
	p.FSMC_AccessMode = FSMC_AccessMode_A;
	FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;
	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
	FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;
	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;
	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p;
	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);
}
//
#define MIN_X	0
#define MIN_Y	0
//
static uint16_t MAX_X = 479, MAX_Y = 319;
//
void uDelay(const uint32_t usec)
{
  uint32_t count = 0;
  const uint32_t utime = (120 * usec / 7);
  do
  {
    if (++count > utime)
    {
      return ;
    }
  }
  while (1);
}
//
void mDelay(const uint32_t msec)
{
	uDelay(msec * 1000);
}
//
inline void TFT_Send_Cmd(uint8_t cmd)
{
	LCD_REG = cmd;
}
//
inline uint8_t TFT_Read_REG(void)
{
	return LCD_REG;
}
//
inline void TFT_Write_Data(uint16_t data)
{
	LCD_RAM = data;
}
//
inline uint16_t TFT_Read_Data(void)
{
	return LCD_RAM;
}
//
void TFT_SetOrientation(uint8_t orient)
{
	TFT_Send_Cmd(0x36);
	switch(orient)
	{

		case 0: TFT_Write_Data(0x08); MAX_X = 319; MAX_Y = 479; break;
		case 1: TFT_Write_Data(0x68); MAX_X = 479; MAX_Y = 319; break;
		case 2: TFT_Write_Data(0xC8); MAX_X = 319; MAX_Y = 479; break;
		case 3: TFT_Write_Data(0xA8); MAX_X = 479; MAX_Y = 319; break;
		default: TFT_Write_Data(0x68); MAX_X = 479; MAX_Y = 319; break;

	}
}
//
uint32_t TFT_ReadID(void)
{
	unsigned char buf[4];
	TFT_Send_Cmd(0xD3);
	buf[0] = LCD_RAM;
	buf[1] = LCD_RAM;
	buf[2] = LCD_RAM;
	buf[3] = LCD_RAM;
	return (buf[1]<<16)+(buf[2]<<8)+(buf[3]);
}

#define LCD_Write_COM(a)	TFT_Send_Cmd(a)
#define LCD_Write_DATA(a)	TFT_Write_Data(a)

void TFT_Init(uint8_t orient)
{

	//
	LCD_GPIO_Config();
	LCD_FSMC_Config();
	mDelay(1);
	//GPIO_ResetBits(TFT_RST);
	GPIO_ResetBits(GPIOB, GPIO_Pin_5);
	mDelay(5);
	//GPIO_SetBits(TFT_RST);
	GPIO_SetBits(GPIOB, GPIO_Pin_5);
	mDelay(120);
	//
	//TFT_Send_Cmd(0x01);		//Software Reset
	//mDelay(1000);
	//
	LCD_Write_COM(0x11);		// Sleep OUT
	mDelay(50);
	LCD_Write_COM(0xC0);		// Power Control 1
	LCD_Write_DATA(0x0d);
	LCD_Write_DATA(0x0d);
	//
	LCD_Write_COM(0xC1);		// Power Control 2
	//LCD_Write_DATA(0x43);
	LCD_Write_DATA(0x47);
	LCD_Write_DATA(0x00);
	//
	LCD_Write_COM(0xC2);		// Power Control 3
	LCD_Write_DATA(0x00);
	//
	LCD_Write_COM(0xC5);		// VCOM Control
	LCD_Write_DATA(0x00);
	LCD_Write_DATA(0x48);
	//
	LCD_Write_COM(0xB6);		// Display Function Control
	LCD_Write_DATA(0x00);
	LCD_Write_DATA(0x42);//	LCD_Write_DATA(0x22);		// 0x42 = Rotate display 180 deg.
	LCD_Write_DATA(0x3B);
	//
	LCD_Write_COM(0xE0);		// PGAMCTRL (Positive Gamma Control)
	LCD_Write_DATA(0x0f);
	LCD_Write_DATA(0x24);
	LCD_Write_DATA(0x1c);
	LCD_Write_DATA(0x0a);
	LCD_Write_DATA(0x0f);
	LCD_Write_DATA(0x08);
	LCD_Write_DATA(0x43);
	LCD_Write_DATA(0x88);
	LCD_Write_DATA(0x32);
	LCD_Write_DATA(0x0f);
	LCD_Write_DATA(0x10);
	LCD_Write_DATA(0x06);
	LCD_Write_DATA(0x0f);
	LCD_Write_DATA(0x07);
	LCD_Write_DATA(0x00);
	//
	LCD_Write_COM(0xE1);		// NGAMCTRL (Negative Gamma Control)
	LCD_Write_DATA(0x0F);
	LCD_Write_DATA(0x38);
	LCD_Write_DATA(0x30);
	LCD_Write_DATA(0x09);
	LCD_Write_DATA(0x0f);
	LCD_Write_DATA(0x0f);
	LCD_Write_DATA(0x4e);
	LCD_Write_DATA(0x77);
	LCD_Write_DATA(0x3c);
	LCD_Write_DATA(0x07);
	LCD_Write_DATA(0x10);
	LCD_Write_DATA(0x05);
	LCD_Write_DATA(0x23);
	LCD_Write_DATA(0x1b);
	LCD_Write_DATA(0x00);
	//
	LCD_Write_COM(0x20);		// Display Inversion OFF
	LCD_Write_DATA(0x00);//C8
	//
	TFT_SetOrientation(orient);//выбираем ориентацию дисплея
	LCD_Write_COM(0x3A);		// Interface Pixel Format
	LCD_Write_DATA(0x55);
	//Display On
	TFT_Send_Cmd(0x29);
}
//
inline void TFT_Send_Data(uint16_t data)
{
	uint8_t data1 = data >> 8;
	uint8_t data2 = data & 0xff;
	TFT_Write_Data(data1);
	TFT_Write_Data(data2);
}
//
inline void TFT_Set_Column(uint16_t start_column,uint16_t end_colunm)
{//задаём рабочую область по x
	TFT_Send_Cmd(0x2A);                                                     
	TFT_Send_Data(start_column);
	TFT_Send_Data(end_colunm);
}
//
void TFT_Set_Page(uint16_t start_page,uint16_t end_page)
{//задаём рабочую область по y
	TFT_Send_Cmd(0x2B);                                                      
	TFT_Send_Data(start_page);
	TFT_Send_Data(end_page);
}
//
void TFT_Set_XY(uint16_t x, uint16_t y)
{
	TFT_Set_Column(x, x);
	TFT_Set_Page(y, y);
	TFT_Send_Cmd(0x2c);
}
//
void TFT_Draw_Pixel(uint16_t x, uint16_t y,uint16_t color)
{
	TFT_Set_XY(x, y);
	TFT_Send_Data(color);
}
//
void TFT_BITMAP(uint16_t x, uint16_t y, uint16_t size_x, uint16_t size_y, uint16_t *bitmap)
{
	TFT_Set_Column(x, x+size_x-1);	//задаём рабочую область по x
	TFT_Set_Page(y, y+size_y-1);	//задаём рабочую область по y
	TFT_Send_Cmd(0x2c);			   //будем писать в видео ОЗУ
	int xy = size_x * size_y;
	int i;
	for(i=0; i<xy; i++)
	{
		#ifdef TFT_LCD16BIT
		TFT_Write_Data(*bitmap++);
		#else
		TFT_Send_Data(*bitmap++);
		#endif
	}
}
//
