#ifndef IWDG_H_
#define IWDG_H_

#include "stm32f10x.h"
#include "stdio.h"

#define IWDG_DEFAULT_PRESCALER IWDG_Prescaler_4	//MIN 0.1ms MAX 409.6ms
#define IWDG_DEFAULT_RELOAD 0x7FF

void IWDGSetup(uint8_t prescaler = IWDG_DEFAULT_PRESCALER);
void IWDGStart(uint16_t reload);
void IWDGStop();
void IWDGReset(uint16_t reload = IWDG_DEFAULT_RELOAD);


#endif
