#ifndef USART_H
#define USART_H

#include "stm32f10x.h"
#include "stdio.h"

class USART{
public:

	void setup();

private:

};

void USART1Setup(uint32_t baud);
void USART1ITSetup(uint16_t flag);

void USART2Setup(uint32_t baud);
void USART2ITSetup(uint16_t flag);


#endif
