#include "NCP5359.h"

void NCP5359::setup(TIM &pwm1Set,TIM &pwm2Set){
	this->pwm[0] = &pwm1Set;
	this->pwm[1]  = &pwm2Set;

	duty(0);

	dutySetTime = 0;

	limitp = 0.99;
	limitm = -0.99;
	System::cycleFunctionNumber++;
}

void NCP5359::cycle(){
	System::cycleFunctionCnt--;

	if(millis() - dutySetTime > TIME_OUT)duty(0);
}

void NCP5359::dutyLimit(float dutym,float dutyp){
	//if(dutyp < limitp)limitp = dutyp;
	//if(dutym > limitm)limitm = dutym;
	limitp = dutyp;
	limitm = dutym;
}

void NCP5359::dutyLimit(float duty){
	//if(duty < limitp)limitp = duty;
	//if(-duty > limitm)limitm = -duty;
	limitp = duty;
	limitm = -duty;
}

void NCP5359::duty(float duty){
	dutySetTime = millis();

	duty = floatlimit(limitp,duty,limitm);
	outDuty = duty;
	if(duty > 0){
		pwm[0]->duty(duty * pwm[0]->pwm_period);
		pwm[1]->duty(0);
	}else{
		pwm[1]->duty(-duty * pwm[0]->pwm_period);
		pwm[0]->duty(0);
	}

}

/*
void NCP5359::setup(TIM &pwmSet,GPIO &gpioSet){
	this->pwm = &pwmSet;
	this->cw  = &gpioSet;

	duty(0);

	dutySetTime = 0;

	limitp = 0.99;
	limitm = -0.99;
	System::cycleFunctionNumber++;
}

void NCP5359::cycle(){
	System::cycleFunctionCnt--;

	if(millis() - dutySetTime > TIME_OUT)duty(0);
}

void NCP5359::dutyLimit(float dutym,float dutyp){
	//if(dutyp < limitp)limitp = dutyp;
	//if(dutym > limitm)limitm = dutym;
	limitp = dutyp;
	limitm = dutym;
}

void NCP5359::dutyLimit(float duty){
	//if(duty < limitp)limitp = duty;
	//if(-duty > limitm)limitm = -duty;
	limitp = duty;
	limitm = -duty;
}

void NCP5359::duty(float duty){
	dutySetTime = millis();

	if(duty > 0){
		cw->write(1);
	}else{
		cw->write(0);
	}

	duty = floatlimit(limitp,duty,limitm);
	outDuty = duty;
	if(duty < 0)duty = -duty;
	pwm->duty(pwm->pwm_period - duty * pwm->pwm_period);
}
*/
