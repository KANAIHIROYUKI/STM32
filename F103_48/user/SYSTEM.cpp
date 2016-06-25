#include "SYSTEM.h"

extern "C" void SysTick_Handler(void){
	if(TimingDelay != 0){
		TimingDelay--;
	}
}

void delay(uint32_t nTime){
	TimingDelay = nTime;
	while(TimingDelay != 0);
}
