#ifndef SYSTEM_H_
#define SYSTEM_H_

#include "stm32f10x.h"
#include "stdio.h"

#include "GPIO.h"
#include "TIM.h"
#include "CAN.h"
#include "ADC.h"
#include "USART.h"
#include "SPI.h"

void setup();

void delay(uint32_t nTime);
uint32_t millis();


#endif
