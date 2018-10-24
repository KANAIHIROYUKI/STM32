#ifndef _SYSTIME_H_
#define _SYSTIME_H_

#include "stm32f10x.h"
#include <stdio.h>


#define TIME_SPLIT 100000 	// �^�C�}�̍ŏ�����\�����߂額ŏ�����\ = 1/TIME_SPLIT [s]

void delay(uint32_t nTime);	//���̊֐��̎��Ԃ͂���قǐ��m�łȂ�
void delayMicros(uint32_t nTime);
uint64_t millis();
uint64_t micros();


#endif
