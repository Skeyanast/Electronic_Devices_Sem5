#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"

// Определение пинов для светодиодов
#define LED_GPIO_PORT GPIOA
#define LED1_PIN GPIO_Pin_0
#define LED2_PIN GPIO_Pin_1
#define LED3_PIN GPIO_Pin_2
#define LED4_PIN GPIO_Pin_3

// Прототипы функций
void LED_Init(void);
void TIM6_Init(void);

// Глобальные переменные
volatile uint16_t counter = 0;

int main(void) {
    LED_Init();
    TIM6_Init();

    while (1) {
        // Обновление состояния светодиодов
        if (counter & 0x01)
            GPIO_SetBits(LED_GPIO_PORT, LED1_PIN);
        else
            GPIO_ResetBits(LED_GPIO_PORT, LED1_PIN);

        if (counter & 0x02)
            GPIO_SetBits(LED_GPIO_PORT, LED2_PIN);
        else
            GPIO_ResetBits(LED_GPIO_PORT, LED2_PIN);

        if (counter & 0x04)
            GPIO_SetBits(LED_GPIO_PORT, LED3_PIN);
        else
            GPIO_ResetBits(LED_GPIO_PORT, LED3_PIN);

        if (counter & 0x08)
            GPIO_SetBits(LED_GPIO_PORT, LED4_PIN);
        else
            GPIO_ResetBits(LED_GPIO_PORT, LED4_PIN);
    }
}

void LED_Init(void) {
    // Включение тактирования порта GPIO
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    // Настройка пинов GPIO для светодиодов
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = LED1_PIN | LED2_PIN | LED3_PIN | LED4_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(LED_GPIO_PORT, &GPIO_InitStructure);
}

void TIM6_Init(void) {
    // Включение тактирования таймера TIM6
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

    // Настройка таймера TIM6
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_Prescaler = 84 - 1; // Таймер работает на частоте 1 кГц
    TIM_TimeBaseInitStructure.TIM_Period = 1000 - 1; // Период 1 секунда
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseInitStructure);

    // Включение прерываний таймера TIM6
    NVIC_SetPriority(TIM6_DAC_IRQn, 0);
    NVIC_EnableIRQ(TIM6_DAC_IRQn);

    TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);

    // Включение таймера TIM6
    TIM_Cmd(TIM6, ENABLE);
}

void TIM6_DAC_IRQHandler(void) {
    if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET) {
        TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
        counter++;
    }
}
