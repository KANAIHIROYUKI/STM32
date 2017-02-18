#include "app.h"

void MOTOR::setup(TIM &PWM_1,TIM &PWM_2){
	this->PWM1 = &PWM_1;
	this->PWM2 = &PWM_2;
}

void MOTOR::test(){
	PWM1->duty(500);
	PWM2->duty(524);
}

void MOTOR::duty(int32_t motorDuty){
	//outDuty = uint32_t(PWM1->pwm_period * motorDuty);
	uint16_t dutyMax = (PWM1->pwm_period/2) - 50;
	if(motorDuty >  dutyMax)motorDuty = dutyMax;
	if(motorDuty < -dutyMax)motorDuty = -dutyMax;
	PWM1->duty((PWM1->pwm_period / 2) - motorDuty);
	PWM2->duty((PWM2->pwm_period / 2) + motorDuty);
	outDuty = motorDuty;
}
