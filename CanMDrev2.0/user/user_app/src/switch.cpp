#include "switch.h"

void Switch::setup(GPIO &gpio_in){
	this->gpio = &gpio_in;
	pinStat = gpio->read();
	toleranceTime = 10;
	readStat = 0;

	System::cycleFunctionNumber++;
}

uint16_t Switch::read(){
	readStat = 0;
	return pinStat;
}

void Switch::cycle(){
	uint16_t input;
	input = gpio->read();
	if(pinStat != input){
		if((millis() - changeTime) > toleranceTime){	//‘O‰ñ‚Ì•Ï‰»‚©‚çˆê’èŽžŠÔŒo‚Á‚Ä‚¢‚È‚¢‚ÆŽó‚¯•t‚¯‚È‚¢
			readStat = 1;
			pinStat = input;

			changeTime = millis();
		}

	}

	System::cycleFunctionCnt--;
}
