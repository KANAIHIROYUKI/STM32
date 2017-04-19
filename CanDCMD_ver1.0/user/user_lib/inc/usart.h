#ifndef USART_H_
#define USART_H_

#include "gpio.h"

#include <stdarg.h>
#include <stdlib.h>

#define USART_DEFAULT_BAUD 115200
#define USART_RX_BUFFER_SIZE 128
#define USART_TX_BUFFER_SIZE 128


class USART {
public:
	void setup(USART_TypeDef *usart,uint32_t baud,GPIO_TypeDef* gpio_tx,uint16_t pin_tx,GPIO_TypeDef* gpio_rx,uint16_t pin_rx);

	void send(char c);
	char read();
	char peek();
	uint16_t available();

	void enable();
	void disable();

	void puts(const char *c);
	void printf(const char *format, ...);

	static char usart1RxBuffer[USART_RX_BUFFER_SIZE];
	static uint16_t usart1RxAddress;
	static uint16_t usart1ReadAddress;

	static char usart2RxBuffer[USART_RX_BUFFER_SIZE];
	static uint16_t usart2RxAddress;
	static uint16_t usart2ReadAddress;

	static char usart3RxBuffer[USART_RX_BUFFER_SIZE];
	static uint16_t usart3RxAddress;
	static uint16_t usart3ReadAddress;

	static char usart1TxBuffer[USART_TX_BUFFER_SIZE];
	static uint16_t usart1TxSendAddress;
	static uint16_t usart1TxWriteAddress;

	static char usart2TxBuffer[USART_TX_BUFFER_SIZE];
	static uint16_t usart2TxSendAddress;
	static uint16_t usart2TxWriteAddress;

	static char usart3TxBuffer[USART_TX_BUFFER_SIZE];
	static uint16_t usart3TxSendAddress;
	static uint16_t usart3TxWriteAddress;

	void ioSetup(GPIO_TypeDef* gpio_tx,uint16_t pin_tx,GPIO_TypeDef* gpio_rx,uint16_t pin_rx);
	USART_TypeDef* usart_usart;
};



void USART1Setup(uint32_t baud);
void USART1ITSetup(uint16_t flag);

void USART2Setup(uint32_t baud);
void USART2ITSetup(uint16_t flag);

void USART3Setup(uint32_t baud);
void USART3ITSetup(uint16_t flag);


#endif
