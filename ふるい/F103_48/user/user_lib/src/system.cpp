#include "system.h"

static uint64_t systemTimer = 0;


void setup(){
	SysTick_Config(SystemCoreClock/TIME_SPLIT);
}

void delay(uint32_t nTime){
	uint64_t delayTime = nTime*1000 + micros();
	while(delayTime > micros());
}

void delayMicros(uint32_t nTime){
	uint64_t delayTime = nTime + micros();
	while(delayTime > micros());
}

uint64_t micros(){
	return systemTimer*(1000000/TIME_SPLIT);
}

uint64_t millis(){
	return micros()/1000;
}

extern "C" void SysTick_Handler(void){
	systemTimer++;
}
