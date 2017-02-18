#ifndef IWDG_H_
#define IWDG_H_

#include "stm32f10x.h"
#include "stdio.h"

#define DEFAULT_PRESCALER IWDG_Prescaler_4
#define DEFAULT_RELOAD

void IWDGSetup(uint8_t prescaler = IWDG_Prescaler_4);
void IWDGStart();
void IWDGReset(uint8_t reload);


#endif
