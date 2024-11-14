//
#include "delay.h"
//
void delay_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Period = 0x7FFF;
	TIM_TimeBaseStructure.TIM_Prescaler = 180-1; // 1 MHz
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	TIM_Cmd(TIM3, ENABLE);
}
//
inline void delay_us(uint16_t uSecs)
{
	TIM3->CNT = 0;
	while((uint16_t)(TIM3->CNT) <= uSecs);
}
//
void delay_ms(uint16_t mSecs)
{
	while(mSecs-- != 0) delay_us(1000);
}




//
