#include "SYSTEM.h"

static uint32_t timingDelay;
static uint64_t systemTimer = 0;


void setup(){
	SysTick_Config(SystemCoreClock/100000);//あとでusになるか試す
}


extern "C" void SysTick_Handler(void){
	systemTimer++;
	if(timingDelay != 0){
		timingDelay--;
	}
}

void delay(uint32_t nTime){
	timingDelay = nTime*100;
	while(timingDelay != 0);//実装が雑なので1msの誤差を生じうる
}

uint64_t micros(){
	return systemTimer*10;
}

uint64_t millis(){
	return micros()/1000;
}
