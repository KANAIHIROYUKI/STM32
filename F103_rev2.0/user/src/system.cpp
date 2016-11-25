#include "system.h"

static uint32_t timingDelay;
static uint64_t systemTimer = 0;


void setup(){
	SysTick_Config(SystemCoreClock/100000);//���Ƃ�us�ɂȂ邩����
}


extern "C" void SysTick_Handler(void){
	systemTimer++;
	if(timingDelay != 0){
		timingDelay--;
	}
}

void delay(uint32_t nTime){
	timingDelay = nTime*100;
	while(timingDelay != 0);//�������G�Ȃ̂�1ms�̌덷�𐶂�����
}

uint64_t micros(){
	return systemTimer*10;
}

uint64_t millis(){
	return micros()/1000;
}
