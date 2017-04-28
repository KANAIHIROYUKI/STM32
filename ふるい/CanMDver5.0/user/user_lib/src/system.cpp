#include "system.h"

static uint64_t systemTimer = 0;

//uint16_t System::cycleFunctionCnt = 0;


void systemSetup(){
	SysTick_Config(SystemCoreClock/TIME_SPLIT);
}

void delay(uint32_t nTime){
	uint64_t delayTime = nTime*1000 + micros();
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


/*

void System::setup(){
	cycleFunctionCnt = 0;
}

uint16_t System::cycle(){
	for(int i=0;i<cycleFunctionCnt;i++){
		cycleFunctionPo[i]();
	}
	return 0;
}

void System::functionAdd(void (*cyclePo)()){
	cycleFunctionPo[cycleFunctionCnt] = cyclePo;
	cycleFunctionCnt++;
}*/
