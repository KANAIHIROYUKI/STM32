#include "switch.h"

void Switch::setup(GPIO_TypeDef* gpioSet,uint16_t pinSet,GPIOMode_TypeDef modeSet){
	gpio.setup(gpioSet,pinSet,modeSet);

	pinStat = gpio.read();
	toleranceTime = 10;
	readStat = 0;									//�Z�b�g�A�b�v���ǂ��ł����Ă��ω����������Ƃ͂���Ȃ�

	System::cycleFunctionNumber++;
}

uint16_t Switch::read(){
	readStat = 0;
	return pinStat;
}

uint16_t Switch::gpioRead(){
	return gpio.read();
}

void Switch::cycle(){
	uint16_t input;
	input = gpio.read();
	if(pinStat != input){
		if((millis() - changeTime) > toleranceTime){	//�O��̕ω������莞�Ԍo���Ă��Ȃ��Ǝ󂯕t���Ȃ�
			readStat = 1;
			pinStat = input;
		}
		changeTime = millis();
	}

	System::cycleFunctionCnt--;
}
