#include "motor.h"

void MOTOR::setup(TIM &PWM_1,TIM &PWM_2){
	this->PWM1 = &PWM_1;
	this->PWM2 = &PWM_2;
}

void MOTOR::duty(int32_t motorDuty){
	uint16_t dutyMax = (PWM1->pwm_period/2) - 50;	//スイッチング時間分でduty上限かける
	if(motorDuty >  dutyMax)motorDuty = dutyMax;
	if(motorDuty < -dutyMax)motorDuty = -dutyMax;
	PWM1->duty((PWM1->pwm_period / 2) - motorDuty);
	PWM2->duty((PWM2->pwm_period / 2) + motorDuty);
}
