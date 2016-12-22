#include "iwdg.h"

void IWDGSetup(uint8_t prescaler){
	IWDG_SetPrescaler(prescaler);
}
void IWDGStart(){
	IWDG_Enable();
}
