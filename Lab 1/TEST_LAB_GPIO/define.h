#ifndef DEFINE_H
#define DEFINE_H
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_syscfg.h"
#include "stm32f4xx_adc.h"
#include "stm32f4xx_dac.h"
#include "stm32f4xx_dma.h"
#include "stm32f4xx_usart.h"
#include "misc.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#define TRUE   0xFF
#define FALSE  0
#define STX		'#'
#define	ETX		'$'
#define ENQ		'/'
#define LEN_NAME_MAX	8
#define Command_MAX		4
typedef uint8_t		Boolean;
typedef uint8_t     Byte;
typedef uint16_t    Word;
typedef uint32_t    DWord;
#define LBYTE(w)  (*((Byte*)&w))
#define HBYTE(w)  (*(((Byte*)&w)+1))
#define L2BYTE(w) (*(((Byte*)&w)+2))
#define H2BYTE(w) (*(((Byte*)&w)+3))
#endif
