#include "define.h" //������� ���������� � �����������

void led_ini(void)	//������������� ����������
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	//��������� ������������ ���������������� �����

	GPIO_InitTypeDef GPIO_InitStructure;	//���������� ���������

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	//����������� ����� �� �����
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	//����������� ����� ��� Push-Pull
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //������� ������������ 100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; //��� ��������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;		//������� �����
	GPIO_Init(GPIOA, &GPIO_InitStructure);			//�������������

	GPIO_SetBits(GPIOA, GPIO_Pin_0);				//��������� 1 �� ������
}
//
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
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;	//����������� ����� �� ����
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	//����������� ����� ��� Push-Pull
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //������� ������������ 100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //�������� � �����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;		//������ �����
	GPIO_Init(GPIOB, &GPIO_InitStructure);			//�������������
}


int main(void)
{
	SystemInit();	//��������� ������������ ����������� �� ���������
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);	//������������ ����������

	led_ini();
	key_ini1();
	key_ini2();

	for(;;)

	{
		if ((GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) == 1) || (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3) == 1)) //���� ������� on
		{
			GPIO_SetBits(GPIOA, GPIO_Pin_0);
		}
		else
		{
			GPIO_ResetBits(GPIOA, GPIO_Pin_0);
		}
	}
//A0B0C10C11;
}
