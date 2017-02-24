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

#define TIME_SPLIT 100000 	// タイマの最小分解能を決める｡最小分解能 = 1/TIME_SPLIT [s]

void systemSetup();

void delay(uint32_t nTime);	//この関数の時間はそれほど正確でない
uint64_t millis();
uint64_t micros();

#endif
