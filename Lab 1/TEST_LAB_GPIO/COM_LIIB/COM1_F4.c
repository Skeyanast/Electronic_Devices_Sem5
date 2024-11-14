//
#include "COM1_F4.h"
//
#include <string.h>
//
static volatile uint8_t COM_TX_END = 0;	         // завершения передачи
static volatile uint8_t COM_RX_END = 0;
static volatile uint16_t RXcount = 0;
//
static uint8_t bufferTX[UART1_BUFFER_SIZE_TX];
static uint8_t bufferRX[UART1_BUFFER_SIZE_RX];
//
//
void COM1_Ini(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_AHB1PeriphClockCmd(UART1_DMA_PINCLOCK, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	//
	GPIO_PinAFConfig(UART1_DMA_PORT, UART1_DMA_TX_SOURCE, GPIO_AF_USART1);
	GPIO_PinAFConfig(UART1_DMA_PORT, UART1_DMA_RX_SOURCE, GPIO_AF_USART1);
	//
	GPIO_InitTypeDef GPIO_InitStructure;
	//
	GPIO_InitStructure.GPIO_Pin = UART1_DMA_TX_PIN | UART1_DMA_RX_PIN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(UART1_DMA_PORT, &GPIO_InitStructure);
	//
	USART_OverSampling8Cmd(USART1, ENABLE);
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = UART1_DMA_COM_BAUD;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	USART_Cmd(USART1, ENABLE);
	//
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = UART1_Priority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream7_IRQn; // TX
	NVIC_Init(&NVIC_InitStructure);
	//
}
//
uint8_t COM1_RX(uint8_t *data, uint16_t *count)
{
	if(COM_RX_END != 0)
	{
		*count = RXcount;
		memcpy(data, bufferRX, RXcount+1);
		return 1;
	}
	else
		return 0;
}
//
void COM1_RX_Stop(void)
{
	USART_ITConfig(USART1, USART_IT_IDLE, DISABLE);
	DMA_Cmd(DMA2_Stream2, DISABLE);
}
//
void COM1_RX_Start(void)
{
	//
	COM_RX_END = 0;
	RXcount = 0;
	//
	DMA_InitTypeDef DMA_InitStructure;
	DMA_Cmd(DMA2_Stream2, DISABLE);
	DMA_DeInit(DMA2_Stream2);
	DMA_InitStructure.DMA_BufferSize = UART1_BUFFER_SIZE_RX;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single ;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_PeripheralBaseAddr =(uint32_t) (&(USART1->DR));
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)bufferRX;
	DMA_Init(DMA2_Stream2, &DMA_InitStructure);
	//
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
	USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
	DMA_Cmd(DMA2_Stream2, ENABLE);
}
//
// COM1_TX = DMA2, Stream7, Channel4  
// COM1_RX = DMA2, Stream2, Channel4
//
void COM1_TX(uint8_t *data, uint16_t count)
{
	if(count == 0) return;
	while(COM_TX_END != 0);
	COM_TX_END = 1;
	memcpy(bufferTX, data, count);
	//
	DMA_InitTypeDef DMA_InitStructure;
	DMA_Cmd(DMA2_Stream7, DISABLE);
	DMA_DeInit(DMA2_Stream7);
	DMA_InitStructure.DMA_BufferSize = count;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single ;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_PeripheralBaseAddr =(uint32_t)(&(USART1->DR));
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High; 
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	DMA_InitStructure.DMA_Memory0BaseAddr =(uint32_t)bufferTX;
	DMA_Init(DMA2_Stream7,&DMA_InitStructure);
	//
	DMA_ITConfig(DMA2_Stream7, DMA_IT_TC, ENABLE);
	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
	DMA_Cmd(DMA2_Stream7, ENABLE);
}
//
void DMA2_Stream7_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA2_Stream7, DMA_IT_TCIF7))
	{
		DMA_ClearITPendingBit(DMA2_Stream7, DMA_IT_TCIF7);
		DMA_ITConfig(DMA2_Stream7, DMA_IT_TC, DISABLE);
		USART_ClearITPendingBit(USART1, USART_IT_TC);
		USART_ITConfig(USART1, USART_IT_TC, ENABLE);
	}
}
//
void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1, USART_IT_IDLE))
	{
		USART_ReceiveData(USART1);
		COM_RX_END = 1;
		USART_ITConfig(USART1, USART_IT_IDLE, DISABLE);
		RXcount = UART1_BUFFER_SIZE_RX - DMA_GetCurrDataCounter(DMA2_Stream2);
		DMA_Cmd(DMA2_Stream2, DISABLE);
		bufferRX[RXcount] = 0;
	}
	//
	if(USART_GetITStatus(USART1, USART_IT_TC))
    {
    	USART_ClearITPendingBit(USART1, USART_IT_TC);
    	USART_ITConfig(USART1, USART_IT_TC, DISABLE);
    	USART_DMACmd(USART1, USART_DMAReq_Tx, DISABLE);
    	DMA_Cmd(DMA2_Stream7, DISABLE);
    	COM_TX_END = 0;
    }
}
//
//
