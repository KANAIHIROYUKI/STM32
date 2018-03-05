#ifndef USART_H_
#define USART_H_

#include "stm32f10x.h"
#include <stdio.h>

#include "gpio.h"
#include "systime.h"

#include <stdarg.h>
#include <stdlib.h>

#define USART_DEFAULT_BAUD 115200
#define USART_RX_BUFFER_SIZE 128
#define USART_TX_BUFFER_SIZE 256

#define USART1_INIT			USART1,PA9,PA10
#define USART2_INIT			USART2,PA2,PA3
#define USART3_INIT			USART3,PB10,PB11

#define USART1_INIT_Remap	USART1,PB6,PB7

class USART {
public:
	void setup(USART_TypeDef *usart,GPIO_TypeDef* gpio_tx,uint16_t pin_tx,GPIO_TypeDef* gpio_rx,uint16_t pin_rx,uint32_t baud);

	void send(char c);
	char read();
	char peek();
	void flush();
	uint16_t available();

	void enable();
	void disable();

	void puts(const char *c);
	void printf(const char *format, ...);

	uint64_t lastReceiveTime();

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

	static uint64_t usart1receiveTime;
	static uint64_t usart2receiveTime;
	static uint64_t usart3receiveTime;

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
