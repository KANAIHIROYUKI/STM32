#include "USART.h"

char USART::usart1RxBuffer[USART_RX_BUFFER_SIZE];
uint16_t USART::usart1RxAddress = 1;
uint16_t USART::usart1ReadAddress = 0;

char USART::usart2RxBuffer[USART_RX_BUFFER_SIZE];
uint16_t USART::usart2RxAddress = 1;
uint16_t USART::usart2ReadAddress = 0;

void USART::setup(USART_TypeDef *usart,uint32_t baud){
	usart_usart = usart;
	if(usart_usart == USART1){
		USART1Setup(baud);
	}else if(usart_usart == USART2){
		USART2Setup(baud);
	}
}

void USART::ITsetup(uint16_t flag){
	if(usart_usart == USART1){
		USART1ITSetup(flag);
	}else if(usart_usart == USART2){
		USART2ITSetup(flag);
	}
}

void USART::send(char c){
	while(USART_GetFlagStatus(usart_usart,USART_FLAG_TXE) == 0);
	USART_SendData(usart_usart,c);
}


char USART::read(){
	if(usart_usart == USART1){
		if(usart1RxAddress == usart1ReadAddress + 1){
			return 0;
		}else{
			if(usart1ReadAddress == USART_RX_BUFFER_SIZE - 1){
				usart1ReadAddress = 0;

			}else{
				usart1ReadAddress++;
			}
			return usart1RxBuffer[usart1ReadAddress];
		}

	}else if(usart_usart == USART2){
		if(usart2RxAddress == usart2ReadAddress + 1){
			return 0;
		}else{
			if(usart2ReadAddress == USART_RX_BUFFER_SIZE - 1){
				usart2ReadAddress = 0;

			}else{
				usart2ReadAddress++;
			}
			return usart2RxBuffer[usart2ReadAddress];
		}
	}
	return 0;
}

char USART::peek(){
	return usart1RxBuffer[usart1ReadAddress];
}

uint16_t USART::available(){
	if(usart1RxAddress <= usart1ReadAddress){
		return usart1RxAddress + USART_RX_BUFFER_SIZE - usart1ReadAddress - 1;
	}else{
		return usart1RxAddress - usart1ReadAddress - 1;
	}
}

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



extern "C" void USART1_IRQHandler(void){
	if(USART_GetITStatus(USART1,USART_IT_RXNE) == SET){
		uint8_t i = USART_ReceiveData(USART1);
		USART_SendData(USART1,i);

		USART::usart1RxBuffer[USART::usart1RxAddress] = USART_ReceiveData(USART1);
		USART::usart1RxAddress++;
		if(USART::usart1RxAddress == USART_RX_BUFFER_SIZE){
			USART::usart1RxAddress = 0;
		}
	}
}


extern "C" void USART2_IRQHandler(void){
	if(USART_GetITStatus(USART2,USART_IT_RXNE) == SET){
		uint8_t i = USART_ReceiveData(USART2);
		USART_SendData(USART2,i);

		USART::usart2RxBuffer[USART::usart2RxAddress] = USART_ReceiveData(USART2);
		USART::usart2RxAddress++;
		if(USART::usart2RxAddress == USART_RX_BUFFER_SIZE){
			USART::usart2RxAddress = 0;
		}
	}
}
