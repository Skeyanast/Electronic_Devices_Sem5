//
#ifndef COM1_H
#define COM1_H
//
#include "define.h"
//
//--------------------------------------------------------------
//
#define  UART1_DMA_COM_BAUD     9600
#define  UART1_DMA_PINCLOCK     RCC_AHB1Periph_GPIOA
#define  UART1_DMA_PORT   		GPIOB
#define  UART1_DMA_TX_PIN       GPIO_Pin_6
#define  UART1_DMA_TX_SOURCE    GPIO_PinSource6
#define  UART1_DMA_RX_PIN       GPIO_Pin_7
#define  UART1_DMA_RX_SOURCE    GPIO_PinSource7
//
#define  UART1_BUFFER_SIZE_RX	64
#define  UART1_BUFFER_SIZE_TX	64
//
#define  UART1_Priority			14
//
void COM1_Ini(void);
void COM1_TX(uint8_t *data, uint16_t count);
void COM1_RX_Start(void);
void COM1_RX_Stop(void);
uint8_t COM1_RX(uint8_t *data, uint16_t *count);
//
#endif
//
