//
#include "GPIO_LIBS.h"
//
static uint16_t TM_GPIO_GetPortSource(GPIO_TypeDef* GPIOx) {
	/* Get port source number */
	/* Offset from GPIOA                       Difference between 2 GPIO addresses */
	return ((uint32_t)GPIOx - (GPIOA_BASE)) / ((GPIOB_BASE) - (GPIOA_BASE));
}

/* Private functions */
static void TM_GPIO_INT_EnableClock(GPIO_TypeDef* GPIOx) {
	/* Set bit according to the 1 << portsourcenumber */
	RCC->AHB1ENR |= (1 << TM_GPIO_GetPortSource(GPIOx));
}


void GPIO_Ini(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, 
              GPIOMode_TypeDef GPIO_Mode, GPIOOType_TypeDef GPIO_OType, 
			  GPIOSpeed_TypeDef GPIO_Speed, GPIOPuPd_TypeDef GPIO_PuPd)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//
	TM_GPIO_INT_EnableClock(GPIOx);
	//
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode;
	GPIO_InitStructure.GPIO_OType = GPIO_OType;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin;
	GPIO_Init(GPIOx, &GPIO_InitStructure);
}
//
