#include "usart.h"

void send(char c){
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
}

void puts(const char *s){
	 int i;
	 for(i=0;s[i]!=0;i++)send(s[i]);
}


void printf(const char *format, ...) {
	va_list ap;
	va_start(ap, format);

	char buffer[USART_TX_BUFFER_SIZE];
	vsprintf(buffer,format,ap);
	puts(buffer);

	va_end(ap);
	return;
}






void SerialSetup(){

}


void USART1Setup(uint32_t baud){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);

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


void USART1ITSetup(uint16_t flag){
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_ITConfig(USART1,flag,ENABLE);
}

void USART1Put(char ch){
	USART_SendData(USART1,ch);
	//while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == 0);
}

extern "C" void USART1_IRQHandler(void){
	USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
}
