#ifndef SYSTEM_H_
#define SYSTEM_H_

#include "stm32f10x.h"
#include <stdio.h>

#include "systime.h"

#include "gpio.h"
#include "tim.h"
#include "can.h"
#include "adc.h"
#include "usart.h"
#include "spi.h"
#include "dma.h"

#define DEFAULT_PRESCALER IWDG_Prescaler_4	//409.6ms
#define DEFAULT_RELOAD 4095	//ç≈ëÂíl§0.1msíPà 


class System{
public:
	void setup();
	void usartSetup(USART &usart);
	void cycle();
	void addFunc(void (*po)());
	void wdgSetup(uint16_t reload = DEFAULT_RELOAD,uint8_t prescaler = IWDG_Prescaler_4);
	void wdgReset();

	static int16_t cycleFunctionNumber;
	static int16_t cycleFunctionCnt;

	uint16_t usartSetupFlag;
	static uint16_t error;
	USART *system_usart;


};

void IWDGSetup(uint16_t reload = DEFAULT_RELOAD,uint8_t prescaler = IWDG_Prescaler_4);
void IWDGReset();


#endif
