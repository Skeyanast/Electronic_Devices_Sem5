#include "define.h"
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"


int main(void)
{
	GPIO_InitTypeDef gpioConf;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	gpioConf.GPIO_Pin = GPIO_Pin_0;
	gpioConf.GPIO_Speed = GPIO_Speed_100MHz;
	gpioConf.GPIO_Mode = GPIO_Mode_OUT;
	gpioConf.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &gpioConf);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	gpioConf.GPIO_Pin = GPIO_Pin_1;
	gpioConf.GPIO_Speed = GPIO_Speed_100MHz;
	gpioConf.GPIO_Mode = GPIO_Mode_IN;
	gpioConf.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &gpioConf);

	for (;;)
	{
		if (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_1) == 1)
		{
			if (GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_0))
			{
				GPIO_ResetBits(GPIOA, GPIO_Pin_0);
			}
			else
			{
				GPIO_SetBits(GPIOA, GPIO_Pin_0);
			}
		}
	}
}
