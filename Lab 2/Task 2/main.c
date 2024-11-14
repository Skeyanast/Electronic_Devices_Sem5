#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"

// ����������� ����� ��� ���������� � ������
#define LED_GPIO_PORT GPIOA
#define LED_PIN GPIO_Pin_0
#define BUTTON_GPIO_PORT GPIOD
#define BUTTON_PIN GPIO_Pin_1

// ��������� �������
void LED_Init(void);
void BUTTON_Init(void);
void TIM7_Init(void);
void TIM7_IRQHandler(void);

// ���������� ����������
volatile uint16_t countdown = 900; // ��������� �������� ��������� �������
volatile uint8_t countdown_active = 0; // ���� ���������� ��������� �������
uint8_t button_prev_state = 1; // ���������� ��������� ������ (1 - �� ������, �.�. ��������� � VCC)

int main(void) {
    LED_Init();
    BUTTON_Init();
    TIM7_Init();

    while (1) {
        uint8_t button_current_state = GPIO_ReadInputDataBit(BUTTON_GPIO_PORT, BUTTON_PIN);

        // �������� �� ������� ��������� ������ � �������� ������ �� ������ (�������)
        if (button_current_state == Bit_RESET && button_prev_state == Bit_SET) {
            // ������ ���� ������, �������� �������� ������
            countdown = 900;
            countdown_active = 1;
            TIM_Cmd(TIM7, ENABLE); // �������� ������
            GPIO_SetBits(LED_GPIO_PORT, LED_PIN); // �������� ���������
        }

        // ���������� �������� ��������� ������ ��� ������������ ���������
        button_prev_state = button_current_state;

        // ���������� ��������� ���������� �� ����� ��������� �������
        if (countdown_active) {
            if (countdown == 0) {
                countdown_active = 0;
                TIM_Cmd(TIM7, DISABLE); // ��������� ������
                GPIO_ResetBits(LED_GPIO_PORT, LED_PIN); // ��������� ���������
            }
        }
    }
}

void LED_Init(void) {
    // ��������� ������������ ����� GPIO
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    // ��������� ���� ��� ����������
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = LED_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(LED_GPIO_PORT, &GPIO_InitStructure);
}

void BUTTON_Init(void) {
    // ��������� ������������ ����� GPIO
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

    // ��������� ���� ��� ������
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = BUTTON_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(BUTTON_GPIO_PORT, &GPIO_InitStructure);
}

void TIM7_Init(void) {
    // ��������� ������������ ������� TIM7
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);

    // ��������� ������� TIM7
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_Prescaler = 84 - 1; // ������ �������� �� ������� 1 ���
    TIM_TimeBaseInitStructure.TIM_Period = 1000 - 1; // ������ 1 �������
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM7, &TIM_TimeBaseInitStructure);

    // ��������� ���������� ������� TIM7
    NVIC_SetPriority(TIM7_IRQn, 0);
    NVIC_EnableIRQ(TIM7_IRQn);

    TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);
}

void TIM7_IRQHandler(void) {
    if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET) {
        TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
        if (countdown_active) {
            if (countdown > 0) {
                countdown--;
            }
        }
    }
}

