#include "NCP5359.h"

void NCP5359::setup(TIM &pwmSet,GPIO &gpioSet){
	this->pwm = &pwmSet;
	this->cw  = &gpioSet;

	duty(0);

	dutySetTime = 0;

	dutyLimit(0.99);
	System::cycleFunctionNumber++;
}

void NCP5359::cycle(){
	System::cycleFunctionCnt--;

	if(millis() - dutySetTime > TIME_OUT)duty(0);
}

void NCP5359::dutyLimit(float dutym,float dutyp){
	if(dutyp < limitp)limitp = dutyp;
	if(dutym > limitm)limitm = dutym;
}

void NCP5359::dutyLimit(float duty){
	if(duty < limitp)limitp = duty;
	if(-duty > limitm)limitm = duty;
}

void NCP5359::duty(float duty){
	outDuty = duty;
	dutySetTime = millis();

	if(duty > 0){
		cw->write(1);
	}else{
		cw->write(0);
	}

	duty = floatlimit(limitm,duty,limitp);
	if(duty < 0)duty = -duty;
	pwm->duty(pwm->pwm_period - duty * pwm->pwm_period);
}
