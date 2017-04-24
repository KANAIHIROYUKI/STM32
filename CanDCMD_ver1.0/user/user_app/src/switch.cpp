#include "switch.h"

void Switch::setup(GPIO_TypeDef* gpioSet,uint16_t pinSet,GPIOMode_TypeDef modeSet){
	gpio.setup(gpioSet,pinSet,modeSet);

	pinStat = gpio.read();
	toleranceTime = 10;
	readStat = 0;									//セットアップ時どうであっても変化があったとはされない

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
		if((millis() - changeTime) > toleranceTime){	//前回の変化から一定時間経っていないと受け付けない
			readStat = 1;
			pinStat = input;
		}
		changeTime = millis();
	}

	System::cycleFunctionCnt--;
}
