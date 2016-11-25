#ifndef SYSTEM_H_
#define SYSTEM_H_

#include "stm32f10x.h"
#include "stdio.h"

#include "gpio.h"
#include "tim.h"
#include "can.h"
#include "adc.h"
#include "usart.h"
#include "spi.h"

void setup();

void delay(uint32_t nTime);
uint64_t millis();
uint64_t micros();

#endif
