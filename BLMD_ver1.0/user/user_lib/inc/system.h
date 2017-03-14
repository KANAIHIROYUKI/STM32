#ifndef SYSTEM_H_
#define SYSTEM_H_

#include "stm32f10x.h"
#include <stdio.h>

#include "gpio.h"
#include "tim.h"
#include "can.h"
#include "adc.h"
#include "usart.h"
#include "spi.h"
#include "dma.h"

#define TIME_SPLIT 100000 	// �^�C�}�̍ŏ�����\�����߂額ŏ�����\ = 1/TIME_SPLIT [s]

#define DEFAULT_PRESCALER IWDG_Prescaler_4	//409.6ms
#define DEFAULT_RELOAD 4095	//�ő�l�0.1ms�P��


class System{
public:
	void setup();
	void usartSetup(USART &usart);
	void cycle();
	void wdgSetup(uint16_t reload = DEFAULT_RELOAD,uint8_t prescaler = IWDG_Prescaler_4);
	void wdgReset();

	static int16_t cycleFunctionNumber;
	static int16_t cycleFunctionCnt;

	uint16_t usartFlag;
	USART *system_usart;
};


void delay(uint32_t nTime);	//���̊֐��̎��Ԃ͂���قǐ��m�łȂ�
uint64_t millis();
uint64_t micros();

void IWDGSetup(uint16_t reload = DEFAULT_RELOAD,uint8_t prescaler = IWDG_Prescaler_4);
void IWDGReset();


#endif
