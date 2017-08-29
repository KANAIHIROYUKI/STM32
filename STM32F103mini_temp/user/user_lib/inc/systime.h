#ifndef _SYSTIME_H_
#define _SYSTIME_H_

#include "stm32f10x.h"
#include <stdio.h>

#define TIME_SPLIT 100000 	// タイマの最小分解能を決める｡最小分解能 = 1/TIME_SPLIT [s]


static uint64_t systemTimer = 0;

uint64_t _micros(){
	return systemTimer*(1000000/TIME_SPLIT);
}

void _delay(uint32_t nTime){
	uint64_t delayTime = nTime*1000 + _micros();
	while(delayTime > _micros());
}

uint64_t _millis(){
	return _micros()/1000;
}

extern "C" void SysTick_Handler(void){
	systemTimer++;
}

#endif
