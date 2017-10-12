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
	duty = floatlimit(-0.99,duty,0.99);
	outDuty = duty;

	//cw->write(0);

	if(duty > 0){
		cw->write(1);
	}else{
		cw->write(0);
	}

	dutySetTime = millis();
	//pwm->duty(1000 + duty*1000);

	if(duty < 0)duty = -duty;
	pwm->duty(pwm->pwm_period - duty * pwm->pwm_period);
}
