#include "system.h"

uint64_t g_system_timerCnt = 0;

int16_t System::cycleFunctionCnt = 0;
int16_t System::cycleFunctionNumber = 0;

void System::setup(){
	SysTick_Config(SystemCoreClock/TIME_SPLIT);
	usartFlag = 0;
}

void System::usartSetup(USART &usart){
	this->system_usart = &usart;
	usartFlag = 1;
}

/*void System::timerSetup(TIM &timer){
	g_system_timer = &timer;
	g_system_timerFlag = 1;
}*/

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

void delay(uint64_t nTime){
	nTime = nTime + millis();
	while(nTime > millis());
}

uint64_t micros(){
	return systicMicros();
}

uint64_t millis(){
	return micros()/1000;
}

uint64_t systicMicros(){
	return g_system_timerCnt*(1000000/TIME_SPLIT);
}

extern "C" void SysTick_Handler(void){
	g_system_timerCnt++;
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
