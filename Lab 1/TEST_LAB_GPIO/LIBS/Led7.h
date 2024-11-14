//
#ifndef LED7_H
#define LED7_H
//
#include "define.h"
#include "GPIO_LIBS.h"
//
//! DATA Port
#define LED_D0	GPIOC, GPIO_Pin_1
#define LED_D1	GPIOC, GPIO_Pin_2
#define LED_D2	GPIOC, GPIO_Pin_3
#define LED_D3	GPIOC, GPIO_Pin_4
#define LED_D4	GPIOC, GPIO_Pin_5
#define LED_D5	GPIOC, GPIO_Pin_6
#define LED_D6	GPIOC, GPIO_Pin_7
#define LED_D7	GPIOC, GPIO_Pin_8
//
#define LED_E11	GPIOC, GPIO_Pin_9
#define LED_E12	GPIOC, GPIO_Pin_10
#define LED_E13	GPIOC, GPIO_Pin_11
#define LED_E14	GPIOC, GPIO_Pin_12
//
//
//#define LED_T1_TIM2            2
//#define LED_T1_TIM4            4
//#define LED_T1_TIM5            5
//#define LED_T1_TIM6            6
#define LED_T1_TIM7            7
//
#ifdef LED_T1_TIM2
  #define LED_TIM1_CLK            RCC_APB1Periph_TIM2
  #define LED_TIM1_NAME           TIM2
#elif defined LED_T1_TIM4
  #define LED_TIM1_CLK            RCC_APB1Periph_TIM4
  #define LED_TIM1_NAME           TIM4
#elif defined LED_T1_TIM5
  #define LED_TIM1_CLK            RCC_APB1Periph_TIM5
  #define LED_TIM1_NAME           TIM5
#elif defined LED_T1_TIM6
  #define LED_TIM1_CLK            RCC_APB1Periph_TIM6
  #define LED_TIM1_NAME           TIM6
#elif defined LED_T1_TIM7
  #define LED_TIM1_CLK            RCC_APB1Periph_TIM7
  #define LED_TIM1_NAME           TIM7
#endif

// подпрограмма иницилизация индикатора
void Led7_Ini(void); 
// подпрограмма вывода строки символов на индикатор
void Led7_Str1(char *p);
//
void uint_to_led(uint16_t data, char *str);
//
#endif
//
