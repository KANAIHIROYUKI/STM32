#include "USART.h"


void USART2Setup(uint32_t baud){
	RCC_APB1PeriphClockCmd(RCC_APB1ENR_USART2EN,ENABLE);

	USART_InitTypeDef USARTInitStructure;
	USART_StructInit(&USARTInitStructure);
	USARTInitStructure.USART_BaudRate = baud;
	USARTInitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USARTInitStructure.USART_Parity = USART_Parity_No;
	USARTInitStructure.USART_StopBits = USART_CR2_STOP_1;
	USARTInitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART2,&USARTInitStructure);

	USART_Cmd(USART2,ENABLE);
}

void USART1Setup(uint32_t baud){
	RCC_APB2PeriphClockCmd(RCC_APB2ENR_USART1EN,ENABLE);

	USART_InitTypeDef USARTInitStructure;
	USART_StructInit(&USARTInitStructure);
	USARTInitStructure.USART_BaudRate = baud;
	USARTInitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USARTInitStructure.USART_Parity = USART_Parity_No;
	USARTInitStructure.USART_StopBits = USART_CR2_STOP_1;
	USARTInitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1,&USARTInitStructure);

	USART_Cmd(USART1,ENABLE);
}


void USART2ITSetup(uint16_t flag){
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_ITConfig(USART2,flag,ENABLE);
}

void USART1ITSetup(uint16_t flag){
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_ITConfig(USART1,flag,ENABLE);
}
