#ifndef SYSTICK_H
#define SYSTICK_H

#include "stm32f30x_conf.h"

#define TIME_SPLIT 100000
static uint64_t systemTimer = 0;

void SystickSetup(){
	SysTick_Config(SystemCoreClock/TIME_SPLIT);
}

uint64_t micros(){
	return systemTimer*(1000000/TIME_SPLIT);
}

uint64_t millis(){
	return micros()/1000;
}

void delay(uint32_t nTime){
	uint64_t delayTime = nTime*1000 + micros();
	while(delayTime > micros());
}




extern "C" void SysTick_Handler(void){
	systemTimer++;
}


#endif
