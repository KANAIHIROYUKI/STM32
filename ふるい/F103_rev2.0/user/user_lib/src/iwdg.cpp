#include "iwdg.h"

void IWDGSetup(uint8_t prescaler){
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	IWDG_SetPrescaler(prescaler);
}
void IWDGStart(uint16_t reload){
	IWDG_SetReload(reload);
	IWDG_ReloadCounter();
	IWDG_Enable();
}

void IWDGReseet(){
	IWDG_ReloadCounter();
}
