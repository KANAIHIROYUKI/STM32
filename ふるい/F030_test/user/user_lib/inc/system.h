#ifndef SYSTEM_H_
#define SYSTEM_H_

#include "stm32f0xx_conf.h"
#include "stdio.h"

#include "gpio.h"
#include "tim.h"
#include "adc.h"
#include "usart.h"
#include "spi.h"
#include "iwdg.h"

#define TIME_SPLIT 100000 	// �^�C�}�̍ŏ�����\�����߂額ŏ�����\ = 1/TIME_SPLIT [s]

void setup();

void delay(uint32_t nTime);	//���̊֐��̎��Ԃ͂���قǐ��m�łȂ�
uint64_t millis();
uint64_t micros();

#endif
