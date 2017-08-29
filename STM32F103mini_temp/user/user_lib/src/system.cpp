#include "system.h"

int16_t System::cycleFunctionCnt = 0;
int16_t System::cycleFunctionNumber = 0;

void System::setup(){
	SysTick_Config(SystemCoreClock/TIME_SPLIT);
	usartFlag = 0;
}
/*
void System::usartSetup(USART &usart){
	this->system_usart = &usart;
	usartFlag = 1;
}*/

void System::cycle(){
	if(cycleFunctionCnt > 0){
		if(usartFlag){
			wdgSetup(1);

			//system_usart->printf("cycle function error cycleCnt = %d\n\r",cycleFunctionCnt);
			//while(1);
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

void setSystemTimer(TIM &timer){
	g_sysTimer = &timer;
	g_systemTimerTIMSetuped = 1;
}

void delay(uint32_t nTime){
	if(g_systemTimerTIMSetuped == 0){
		_delay(nTime);
	}else{

	}
}

uint64_t micros(){
	if(g_systemTimerTIMSetuped == 0){
		return _micros();
	}else{

	}
}

uint64_t millis(){
	if(g_systemTimerTIMSetuped == 0){
		return _millis();
	}else{

	}
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
