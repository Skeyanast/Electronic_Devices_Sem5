#include "define.h" //базовые библиотеки и определени€

void led_ini1(void)	//инициализаци€ светодиода
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	//включение тактировани€ соответствующего порта

	GPIO_InitTypeDef GPIO_InitStructure;	//объ€вление структуры

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	//настраиваем ножку на выход
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	//настраиваем ножку как Push-Pull
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //частота тактировани€ 100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; //без подт€жки
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;		//нулева€ ножка
	GPIO_Init(GPIOB, &GPIO_InitStructure);			//инициализаци€

	GPIO_SetBits(GPIOB, GPIO_Pin_2);				//установка 1 на выходе
}

void led_ini2(void)	//инициализаци€ светодиода
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);	//включение тактировани€ соответствующего порта

	GPIO_InitTypeDef GPIO_InitStructure;	//объ€вление структуры

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	//настраиваем ножку на выход
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	//настраиваем ножку как Push-Pull
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //частота тактировани€ 100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; //без подт€жки
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;		//нулева€ ножка
	GPIO_Init(GPIOC, &GPIO_InitStructure);			//инициализаци€

	GPIO_SetBits(GPIOC, GPIO_Pin_2);				//установка 1 на выходе
}

void key_ini1(void)//инициализаци€ кнопки
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	//включение тактировани€ соответствующего порта

	GPIO_InitTypeDef GPIO_InitStructure;	//объ€вление структуры

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;	//настраиваем ножку на вход
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	//настраиваем ножку как Push-Pull
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //частота тактировани€ 100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //подт€жка к плюсу
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;		//перва€ ножка
	GPIO_Init(GPIOA, &GPIO_InitStructure);			//инициализаци€
}

void key_ini2(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;	//настраиваем ножку на вход
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	//настраиваем ножку как Push-Pull
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //частота тактировани€ 100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //подт€жка к плюсу
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;		//перва€ ножка
	GPIO_Init(GPIOA, &GPIO_InitStructure);			//инициализаци€
}


int main(void)
{
	SystemInit();	//настройка тактировани€ контроллера по умолчанию
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);	//конфигураци€ прерываний

	led_ini1();
	led_ini2();
	key_ini1();
	key_ini2();

	for(;;)

	{
		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) == 1) //если тумблер on
		{
			GPIO_SetBits(GPIOB, GPIO_Pin_2);
		}
		else
		{
			GPIO_ResetBits(GPIOB, GPIO_Pin_2);
		}

		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_10) == 1)
		{
			GPIO_SetBits(GPIOC, GPIO_Pin_2);
		}
		else
		{
			GPIO_ResetBits(GPIOC, GPIO_Pin_2);
		}
	}
//A0B0C10C11;
}
