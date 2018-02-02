#include "system.h"

int16_t System::cycleFunctionCnt = 0;
int16_t System::cycleFunctionNumber = 0;
uint16_t System::error = 0;

void (*cycleFunc[32])();
uint16_t cycleFuncNum;

void System::setup(){
	SysTick_Config(SystemCoreClock/TIME_SPLIT);
	usartSetupFlag = 0;

	error = 0;
}

void System::usartSetup(USART &usart){
	this->system_usart = &usart;
	usartSetupFlag = 1;
}

void System::cycle(){
	if(error){
		if(usartSetupFlag){
			system_usart->printf("system error = %d\n\r",error);
			delay(10);
			error = 0;
		}
	}

	if(cycleFunctionCnt > 0){
		error = cycleFunctionCnt;

		if(usartSetupFlag){
			wdgSetup(100);
			system_usart->printf("cycle function error cycleCnt = %d\n\r",cycleFunctionCnt);
			delay(10);
			while(1);
		}
	}
	wdgReset();
	cycleFunctionCnt = cycleFunctionNumber;

	for(int i=0;i<cycleFuncNum;i++){
		cycleFunc[i]();
	}
}

void System::wdgSetup(uint16_t reload,uint8_t prescaler){
	IWDGSetup(reload,prescaler);
}

void System::wdgReset(){
	IWDGReset();
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
