//
#ifndef GPIO_LIBS_H
#define GPIO_LIBS_H
//
#include "define.h"
//
// работа с портами
/*
#define Pin2 	GPIOC, GPIO_Pin_6
				  ^ 	  ^
			 port |       |
						 bit
						 
GPIO_Ini(Pin2, GPIOMode, GPIO_OType, GPIO_Speed, GPIO_PuPd)

SETPORT(Pin2)
              
*/

#define SETPORT(port_bit)	GPIO_SetBits(port_bit)
#define CLRPORT(port_bit)	GPIO_ResetBits(port_bit)
#define TSTPORT(port_bit)	GPIO_ReadInputDataBit(port_bit)
//
void GPIO_Ini(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, 
              GPIOMode_TypeDef GPIO_Mode, GPIOOType_TypeDef GPIO_OType, 
			  GPIOSpeed_TypeDef GPIO_Speed, GPIOPuPd_TypeDef GPIO_PuPd);

//
#endif
//
