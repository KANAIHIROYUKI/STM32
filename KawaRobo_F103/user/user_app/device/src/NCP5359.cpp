#include "NCP5359.h"

void NCP5359::setup(TIM &pwmSet,GPIO &gpioSet){
	this->pwm = &pwmSet;
	this->cw  = &gpioSet;

	duty(0);

	dutySetTime = 0;

	System::cycleFunctionNumber++;
}

void NCP5359::cycle(){
	System::cycleFunctionCnt--;

	if(millis() - dutySetTime > TIME_OUT)duty(0);
}


void NCP5359::duty(float duty){
	if(duty >= 0){
		cw->write(1);
	}else{
		cw->write(0);
	}

	dutySetTime = millis();
	pwm->dutyF(duty);
}
