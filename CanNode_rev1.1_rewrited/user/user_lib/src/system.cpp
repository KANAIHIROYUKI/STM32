#include "system.h"

static uint64_t systemTimer = 0;
int16_t System::cycleFunctionCnt = 0;
int16_t System::cycleFunctionNumber = 0;

void System::setup(){
	SysTick_Config(SystemCoreClock/TIME_SPLIT);
	usartFlag = 0;
}

void System::usartSetup(USART &usart){
	this->system_usart = &usart;
	usartFlag = 1;
	cycleFunctionCnt = 0;
}

void System::cycle(){
	if(cycleFunctionCnt > 0){
		if(usartFlag){
			system_usart->printf("cycle function error cycleCnt = %d\n\r",cycleFunctionCnt);
			while(1);
		}
	}
	wdgReset();
	cycleFunctionCnt = cycleFunctionNumber;
}

void System::wdgSetup(uint16_t reload,uint8_t prescaler){
	IWDGSetup(reload,prescaler);
}

void System::wdgReset(){
	IWDGReset();
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

void IWDGSetup(uint16_t reload,uint8_t prescaler){
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	IWDG_SetPrescaler(prescaler);
	IWDG_SetReload(reload);
	IWDGReset();
	IWDG_Enable();
}

void IWDGReset(){
	IWDG_ReloadCounter();
}
