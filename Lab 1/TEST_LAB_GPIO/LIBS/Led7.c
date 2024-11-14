//
#include "Led7.h"
//
const uint8_t Led_SimD[10]  = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
//
static volatile uint8_t Led_Char1[5] = {0, 0, 0, 0, 0};
//
void uint_to_led(uint16_t data, char *str)
{
	uint16_t i = data / 100;
	data = data % 100;
	*str++ = i + '0';
	//if(i == 0) *str++ = ' ';
	//else       *str++ = i + '0';
	uint16_t i1 = data / 10;
	data = data % 10;
	*str++ = i1 + '0';
	//if((i == 0) && (i1 == 0)) *str++ = ' ';
	//else *str++ = i1 + '0';
	*str++ = data + '0';
	*str = 0;
}
//
static void LedPORT(uint8_t data)
{
	if((data & 1) != 0) SETPORT(LED_D0);
	else CLRPORT(LED_D0);
	if((data & 2) != 0) SETPORT(LED_D1);
	else CLRPORT(LED_D1);
	if((data & 4) != 0) SETPORT(LED_D2);
	else CLRPORT(LED_D2);
	if((data & 8) != 0) SETPORT(LED_D3);
	else CLRPORT(LED_D3);
	if((data & 16) != 0) SETPORT(LED_D4);
	else CLRPORT(LED_D4);
	if((data & 32) != 0) SETPORT(LED_D5);
	else CLRPORT(LED_D5);
	if((data & 64) != 0) SETPORT(LED_D6);
	else CLRPORT(LED_D6);
	if((data & 128) != 0) SETPORT(LED_D7);
	else CLRPORT(LED_D7);
}
//
void Led7_Ini(void)
{
	//
	GPIO_Ini(LED_D0, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_Speed_100MHz, GPIO_PuPd_NOPULL);
	GPIO_Ini(LED_D1, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_Speed_100MHz, GPIO_PuPd_NOPULL);
	GPIO_Ini(LED_D2, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_Speed_100MHz, GPIO_PuPd_NOPULL);
	GPIO_Ini(LED_D3, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_Speed_100MHz, GPIO_PuPd_NOPULL);
	GPIO_Ini(LED_D4, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_Speed_100MHz, GPIO_PuPd_NOPULL);
	GPIO_Ini(LED_D5, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_Speed_100MHz, GPIO_PuPd_NOPULL);
	GPIO_Ini(LED_D6, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_Speed_100MHz, GPIO_PuPd_NOPULL);
	GPIO_Ini(LED_D7, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_Speed_100MHz, GPIO_PuPd_NOPULL);
	//
	GPIO_Ini(LED_E11, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_Speed_100MHz, GPIO_PuPd_NOPULL);
	GPIO_Ini(LED_E12, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_Speed_100MHz, GPIO_PuPd_NOPULL);
	GPIO_Ini(LED_E13, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_Speed_100MHz, GPIO_PuPd_NOPULL);
	GPIO_Ini(LED_E14, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_Speed_100MHz, GPIO_PuPd_NOPULL);
	//
	LedPORT(0x7F);
	CLRPORT(LED_E11);
	CLRPORT(LED_E12);
	CLRPORT(LED_E13);
	CLRPORT(LED_E14);
	//
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Prescaler = 48-1;
	TIM_TimeBaseStructure.TIM_Period = (100 * (uint32_t)10) - 1; //
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);
	TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE); // включаем прерывания по переполнению
	NVIC_EnableIRQ(TIM7_IRQn);
	TIM_Cmd(TIM7, ENABLE);
}
//
void TIM7_IRQHandler(void)
{
	static uint8_t NInd = 0; // номер индикатора 
	TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
	switch(NInd) // зажеч
	{
		case 0:
			CLRPORT(LED_E14);
			LedPORT(Led_Char1[0]);
			SETPORT(LED_E11);
			NInd++;
			break;
		case 1:
			CLRPORT(LED_E11);
			LedPORT(Led_Char1[1]);
			SETPORT(LED_E12);
			NInd++;
			break;
		case 2:
			CLRPORT(LED_E12);
			LedPORT(Led_Char1[2]);
			SETPORT(LED_E13);
			NInd++;
			break;
		default:
			CLRPORT(LED_E13);
			LedPORT(Led_Char1[3]);
			SETPORT(LED_E14);
			NInd = 0;
			break;
	}
}
//
//
void Led7_Str1(char *p)
{
  int i;
  	TIM_Cmd(TIM7, DISABLE);
	for(i=0; i <= 3; i++)
	{
		if(*p == 0)
		{
			Led_Char1[i] = 0;
		}
		else if((*p >= '0') && (*p <= '9'))
		{
			Led_Char1[i] = Led_SimD[*p - '0'];
			++p;
		}
		else if(*p == '-')
		{
			Led_Char1[i] = 1 << 6;
			++p;
		}
		else if(*p == '_')
		{
			Led_Char1[i] = 1 << 3;
			++p;
		}
		else if((*p == '.') && (i != 0))
		{
			Led_Char1[--i] |= 1 << 7;
			++p;
		}
		else
		{
			Led_Char1[i] = 0;
			++p;
		}
	}
	TIM_Cmd(TIM7, ENABLE);
}
//
//
