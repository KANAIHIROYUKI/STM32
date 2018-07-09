#include "system.h"

int16_t System::cycleFunctionCnt = 0;
int16_t System::cycleFunctionNumber = 0;
uint16_t System::error = 0;

void System::setup(){
	SysTick_Config(SystemCoreClock/TIME_SPLIT);
	usartSetupFlag = 0;
	FLASH_SetLatency(FLASH_Latency_2);
	FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

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
}

void System::wdgSetup(uint16_t reload,uint8_t prescaler){
	IWDGSetup(reload,prescaler);
}

void System::wdgReset(){
	IWDGReset();
}

uint32_t System::flashRead(uint32_t address){	//F103‚Í1ƒy[ƒW1KB‚È‚Ì‚Å
	if(address > 255)return 0;
	return *(uint32_t *)(0x801FC00 + address * 4);
}

bool System::flashWrite(uint32_t address,uint32_t data){
	if(address > 255)return 1;

	while(FLASH_GetFlagStatus(FLASH_BUSY) == SET);

	FLASH_Unlock();
	FLASH_ProgramWord(0x801FC00 + address*4,data);
	while(FLASH_GetFlagStatus(FLASH_BUSY) == SET);
	FLASH_Lock();
	return 0;
}

bool System::flashErase(){

	//while(FLASH_GetFlagStatus(FLASH_BUSY) == SET);
	FLASH_Unlock();
	FLASH_ErasePage(0x801FC00);
	while(FLASH_GetFlagStatus(FLASH_BUSY) == SET)
	FLASH_Lock();
	return 0;
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
