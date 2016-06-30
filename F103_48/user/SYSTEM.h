#ifndef SYSTEM_H_
#define SYSTEM_H_

#include "stm32f10x.h"
#include "stdio.h"

#include "GPIO.h"
#include "TIM.h"
#include "CAN.h"
#include "ADC.h"
#include "USART.h"


void setup();

static uint32_t TimingDelay;
void delay(uint32_t nTime);


#endif
