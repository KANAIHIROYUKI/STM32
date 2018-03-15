#ifndef MAIN_H_
#define MAIN_H_

#include "system.h"
#include "app.h"
#include "util.h"

#define FLASH_PAGE_ADDRESS 0x801F800

System sys;

USART serial;
TIM pwm;
ADC adc;

uint64_t intervalTime;
uint32_t FlashRead(uint32_t address);
void FlashWrite(uint32_t address,uint32_t data);
int FlashErase();

void setup(){
	sys.setup();
	serial.setup(USART1,115200,PA9,PA10);
	serial.printf("DATE = %s\n\r",__DATE__);
	serial.printf("TIME = %s\n\r",__TIME__);

	sys.usartSetup(serial);
	pwm.pwmSetup(TIM1,1,PA8,18000,8);
	pwm.duty(0);
	adc.setup(ADC1,0,PA0);
}



#endif
