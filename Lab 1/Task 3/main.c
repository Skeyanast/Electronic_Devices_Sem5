#include "define.h" //������� ���������� � �����������

void led_ini1(void)	//������������� ����������
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	//��������� ������������ ���������������� �����

	GPIO_InitTypeDef GPIO_InitStructure;	//���������� ���������

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	//����������� ����� �� �����
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	//����������� ����� ��� Push-Pull
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //������� ������������ 100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; //��� ��������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;		//������� �����
	GPIO_Init(GPIOB, &GPIO_InitStructure);			//�������������

	GPIO_SetBits(GPIOB, GPIO_Pin_2);				//��������� 1 �� ������
}

void led_ini2(void)	//������������� ����������
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);	//��������� ������������ ���������������� �����

	GPIO_InitTypeDef GPIO_InitStructure;	//���������� ���������

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	//����������� ����� �� �����
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	//����������� ����� ��� Push-Pull
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //������� ������������ 100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; //��� ��������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;		//������� �����
	GPIO_Init(GPIOC, &GPIO_InitStructure);			//�������������

	GPIO_SetBits(GPIOC, GPIO_Pin_2);				//��������� 1 �� ������
}

void key_ini1(void)//������������� ������
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	//��������� ������������ ���������������� �����

	GPIO_InitTypeDef GPIO_InitStructure;	//���������� ���������

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;	//����������� ����� �� ����
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	//����������� ����� ��� Push-Pull
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //������� ������������ 100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //�������� � �����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;		//������ �����
	GPIO_Init(GPIOA, &GPIO_InitStructure);			//�������������
}

void key_ini2(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;	//����������� ����� �� ����
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	//����������� ����� ��� Push-Pull
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //������� ������������ 100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //�������� � �����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;		//������ �����
	GPIO_Init(GPIOA, &GPIO_InitStructure);			//�������������
}


int main(void)
{
	SystemInit();	//��������� ������������ ����������� �� ���������
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);	//������������ ����������

	led_ini1();
	led_ini2();
	key_ini1();
	key_ini2();

	for(;;)

	{
		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) == 1) //���� ������� on
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
