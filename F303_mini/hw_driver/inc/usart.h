#ifndef USART_H_
#define USART_H_

#include "stm32f30x_conf.h"

#include <stdio.h>
//#include <stdlib.h>

#define USART_DEFAULT_BAUD 115200
#define USART_RX_BUFFER_SIZE 128
#define USART_TX_BUFFER_SIZE 256

void puts(const char *c);
void printf(const char *format, ...);
void send(char ch);

void USART1Setup(uint32_t baud = USART_DEFAULT_BAUD);
void USART1ITSetup(uint16_t flag);
void USART1Put(char ch);

#endif
