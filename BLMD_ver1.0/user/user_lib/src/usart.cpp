#include "usart.h"

char USART::usart1RxBuffer[USART_RX_BUFFER_SIZE];
uint16_t USART::usart1RxAddress = 1;
uint16_t USART::usart1ReadAddress = 0;

char USART::usart2RxBuffer[USART_RX_BUFFER_SIZE];
uint16_t USART::usart2RxAddress = 1;
uint16_t USART::usart2ReadAddress = 0;

char USART::usart3RxBuffer[USART_RX_BUFFER_SIZE];
uint16_t USART::usart3RxAddress = 1;
uint16_t USART::usart3ReadAddress = 0;


char USART::usart1TxBuffer[USART_TX_BUFFER_SIZE];
uint16_t USART::usart1TxSendAddress = 0;
uint16_t USART::usart1TxWriteAddress = 0;

char USART::usart2TxBuffer[USART_TX_BUFFER_SIZE];
uint16_t USART::usart2TxSendAddress = 0;
uint16_t USART::usart2TxWriteAddress = 0;

char USART::usart3TxBuffer[USART_TX_BUFFER_SIZE];
uint16_t USART::usart3TxSendAddress = 0;
uint16_t USART::usart3TxWriteAddress = 0;



void USART::puts(const char *s){
 int i;
 for(i=0;s[i]!=0;i++)send(s[i]);
}

void USART::printf(const char *format, ...) {
	va_list ap;
	va_start(ap, format);

	char buffer[USART_TX_BUFFER_SIZE];
	vsprintf(buffer,format,ap);
	puts(buffer);

	va_end(ap);
	return;
}


void USART::setup(USART_TypeDef *usart,uint32_t baud,GPIO_TypeDef* gpio_tx,uint16_t pin_tx,GPIO_TypeDef* gpio_rx,uint16_t pin_rx){
	usart_usart = usart;
	ioSetup(gpio_tx,pin_tx,gpio_rx,pin_rx);

	if(usart_usart == USART1){
		USART1Setup(baud);
		USART1ITSetup(USART_IT_RXNE);
		USART1ITSetup(USART_IT_TXE);
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
	}else if(usart_usart == USART2){
		USART2Setup(baud);
		USART2ITSetup(USART_IT_RXNE);
		USART2ITSetup(USART_IT_TXE);
		USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
	}else if(usart_usart == USART3){
		USART3Setup(baud);
		USART3ITSetup(USART_IT_RXNE);
		USART3ITSetup(USART_IT_TXE);
		USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
	}
}


void USART::send(char c){

	if(usart_usart == USART1){
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);

		if(USART::usart1TxSendAddress == USART::usart1TxWriteAddress + 1){
			USART::usart1TxSendAddress++;
			if(USART::usart1TxSendAddress >= USART_TX_BUFFER_SIZE)USART::usart1TxSendAddress = 0;
		}
		USART::usart1TxBuffer[USART::usart1TxWriteAddress] = c;
		USART::usart1TxWriteAddress++;
		if(USART::usart1TxWriteAddress >= USART_TX_BUFFER_SIZE){
			USART::usart1TxWriteAddress = 0;
		}

	}else if(usart_usart == USART2){
		USART_ITConfig(USART2, USART_IT_TXE, ENABLE);

		if(USART::usart2TxSendAddress == USART::usart2TxWriteAddress + 1){
			USART::usart2TxSendAddress++;
			if(USART::usart2TxSendAddress >= USART_TX_BUFFER_SIZE)USART::usart2TxSendAddress = 0;
		}
		USART::usart2TxBuffer[USART::usart2TxWriteAddress] = c;
		USART::usart2TxWriteAddress++;
		if(USART::usart2TxWriteAddress >= USART_TX_BUFFER_SIZE){
			USART::usart2TxWriteAddress = 0;
		}

	}else if(usart_usart == USART3){
		USART_ITConfig(USART3, USART_IT_TXE, ENABLE);

		if(USART::usart3TxSendAddress == USART::usart3TxWriteAddress + 1){
			USART::usart3TxSendAddress++;
			if(USART::usart3TxSendAddress >= USART_TX_BUFFER_SIZE)USART::usart3TxSendAddress = 0;
		}
		USART::usart3TxBuffer[USART::usart3TxWriteAddress] = c;
		USART::usart3TxWriteAddress++;
		if(USART::usart3TxWriteAddress >= USART_TX_BUFFER_SIZE){
			USART::usart3TxWriteAddress = 0;
		}

	}
}


char USART::read(){
	if(usart_usart == USART1){
		if(usart1RxAddress == usart1ReadAddress + 1){	//読めるデータがない時
			return 0;
		}else{
			if(usart1ReadAddress == USART_RX_BUFFER_SIZE - 1){	//
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
	}else if(usart_usart == USART3){
		if(usart3RxAddress == usart3ReadAddress + 1){
			return 0;
		}else{
			if(usart3ReadAddress == USART_RX_BUFFER_SIZE - 1){
				usart3ReadAddress = 0;

			}else{
				usart3ReadAddress++;
			}
			return usart3RxBuffer[usart3ReadAddress];
		}
	}
	return 0;
}

char USART::peek(){
	if(usart_usart == USART1){
		return usart1RxBuffer[usart1ReadAddress + 1];
	}else if(usart_usart == USART2){
		return usart2RxBuffer[usart2ReadAddress + 1];
	}else if(usart_usart == USART3){
		return usart3RxBuffer[usart3ReadAddress + 1];
	}
	return 0;
}

uint16_t USART::available(){
	if(usart_usart == USART1){
		if(usart1RxAddress <= usart1ReadAddress){
			return usart1RxAddress + USART_RX_BUFFER_SIZE - usart1ReadAddress - 1;
		}else{
			return usart1RxAddress - usart1ReadAddress - 1;
		}
	}else if(usart_usart == USART2){
		if(usart2RxAddress <= usart2ReadAddress){
			return usart2RxAddress + USART_RX_BUFFER_SIZE - usart2ReadAddress - 1;
		}else{
			return usart2RxAddress - usart2ReadAddress - 1;
		}
	}else if(usart_usart == USART3){
		if(usart3RxAddress <= usart3ReadAddress){
			return usart3RxAddress + USART_RX_BUFFER_SIZE - usart3ReadAddress - 1;
		}else{
			return usart3RxAddress - usart3ReadAddress - 1;
		}
	}
	return 0;

}

void USART::ioSetup(GPIO_TypeDef* gpio_tx,uint16_t pin_tx,GPIO_TypeDef* gpio_rx,uint16_t pin_rx){
	GPIO TX,RX;
	TX.setup(gpio_tx,pin_tx,OUTPUT_AF);
	RX.setup(gpio_rx,pin_rx,INPUT_PU);
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

void USART3Setup(uint32_t baud){
	RCC_APB1PeriphClockCmd(RCC_APB1ENR_USART3EN,ENABLE);

	USART_InitTypeDef USARTInitStructure;
	USART_StructInit(&USARTInitStructure);
	USARTInitStructure.USART_BaudRate = baud;
	USARTInitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USARTInitStructure.USART_Parity = USART_Parity_No;
	USARTInitStructure.USART_StopBits = USART_CR2_STOP_1;
	USARTInitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART3,&USARTInitStructure);

	USART_Cmd(USART3,ENABLE);
}





void USART1ITSetup(uint16_t flag){
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_ITConfig(USART1,flag,ENABLE);
}

void USART2ITSetup(uint16_t flag){
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_ITConfig(USART2,flag,ENABLE);
}

void USART3ITSetup(uint16_t flag){
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_ITConfig(USART3,flag,ENABLE);
}

extern "C" void USART1_IRQHandler(void){
	if(USART_GetITStatus(USART1,USART_IT_RXNE) == SET){//受信割込み
		uint8_t i = USART_ReceiveData(USART1);
		USART_SendData(USART1,i);

		USART::usart1RxBuffer[USART::usart1RxAddress] = USART_ReceiveData(USART1);
		USART::usart1RxAddress++;
		if(USART::usart1RxAddress >= USART_RX_BUFFER_SIZE){
			USART::usart1RxAddress = 0;
		}
	}else if(USART_GetITStatus(USART1,USART_IT_TXE)){//送信完了割込み
		if(USART::usart1TxWriteAddress != USART::usart1TxSendAddress){//バッファに送信すべきデータが有る
			USART_SendData(USART1,USART::usart1TxBuffer[USART::usart1TxSendAddress]);
			USART::usart1TxSendAddress++;
			if(USART::usart1TxSendAddress >= USART_TX_BUFFER_SIZE)USART::usart1TxSendAddress = 0;
		}else{
			USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
		}
	}
}


extern "C" void USART2_IRQHandler(void){
	if(USART_GetITStatus(USART2,USART_IT_RXNE) == SET){//受信割込み
		uint8_t i = USART_ReceiveData(USART2);
		USART_SendData(USART2,i);

		USART::usart2RxBuffer[USART::usart2RxAddress] = USART_ReceiveData(USART2);
		USART::usart2RxAddress++;
		if(USART::usart2RxAddress >= USART_RX_BUFFER_SIZE){
			USART::usart2RxAddress = 0;
		}
	}else if(USART_GetITStatus(USART2,USART_IT_TXE)){//送信完了割込み
		if(USART::usart2TxWriteAddress != USART::usart2TxSendAddress){//バッファに送信すべきデータが有る
			USART_SendData(USART2,USART::usart2TxBuffer[USART::usart2TxSendAddress]);
			USART::usart2TxSendAddress++;
			if(USART::usart2TxSendAddress >= USART_TX_BUFFER_SIZE)USART::usart2TxSendAddress = 0;
		}else{
			USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
		}
	}
}


extern "C" void USART3_IRQHandler(void){
	if(USART_GetITStatus(USART3,USART_IT_RXNE) == SET){//受信割込み
		uint8_t i = USART_ReceiveData(USART3);
		USART_SendData(USART3,i);

		USART::usart3RxBuffer[USART::usart3RxAddress] = USART_ReceiveData(USART3);
		USART::usart3RxAddress++;
		if(USART::usart3RxAddress >= USART_RX_BUFFER_SIZE){
			USART::usart3RxAddress = 0;
		}
	}else if(USART_GetITStatus(USART3,USART_IT_TXE)){//送信完了割込み
		if(USART::usart3TxWriteAddress != USART::usart3TxSendAddress){//バッファに送信すべきデータが有る
			USART_SendData(USART3,USART::usart3TxBuffer[USART::usart3TxSendAddress]);
			USART::usart3TxSendAddress++;
			if(USART::usart3TxSendAddress >= USART_TX_BUFFER_SIZE)USART::usart3TxSendAddress = 0;
		}else{
			USART_ITConfig(USART3, USART_IT_TXE, DISABLE);
		}
	}
}
