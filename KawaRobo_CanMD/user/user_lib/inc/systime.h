#ifndef _SYSTIME_H_
#define _SYSTIME_H_

#include "stm32f10x.h"
#include <stdio.h>


#define TIME_SPLIT 100000 	// タイマの最小分解能を決める｡最小分解能 = 1/TIME_SPLIT [s]

void delay(uint32_t nTime);	//この関数の時間はそれほど正確でない
void delayMicros(uint32_t nTime);
uint64_t millis();
uint64_t micros();


#endif
