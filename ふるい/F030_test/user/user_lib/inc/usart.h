#ifndef USART_H_
#define USART_H_

#include "stm32f0xx_conf.h"
#include "stdio.h"

#include <stdarg.h>
#include <stdlib.h>

#define USART_DEFAULT_BAUD 115200
#define USART_RX_BUFFER_SIZE 128
#define USART_TX_BUFFER_SIZE 128


class USART {
public:
	void setup(USART_TypeDef *usart,uint32_t baud = USART_DEFAULT_BAUD);

	void send(char c);
	char read();
	char peek();
	uint16_t available();

	void puts(const char *c);
	void printf(const char *format, ...);

	static char usart1RxBuffer[USART_RX_BUFFER_SIZE];
	static uint16_t usart1RxAddress;
	static uint16_t usart1ReadAddress;

	static char usart1TxBuffer[USART_TX_BUFFER_SIZE];
	static uint16_t usart1TxSendAddress;
	static uint16_t usart1TxWriteAddress;
private:
	USART_TypeDef* usart_usart;
};



void USART1Setup(uint32_t baud);
void USART1ITSetup(uint16_t flag);


#endif
