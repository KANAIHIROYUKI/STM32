#include "motor.h"

void MOTOR::setup(TIM &PWM_1,TIM &PWM_2){
	this->pwm1 = &PWM_1;
	this->pwm2 = &PWM_2;
	duty(0);
	assigneStat = MOTOR_ASSIGNE_NONE;
}
void MOTOR::enPwmAssigne(TIM &PWM_EN){
	this->pwmEn = &PWM_EN;
	assigneStat = MOTOR_ASSIGNE_PWM;
}

void MOTOR::enPinAssigne(GPIO &gpioEn){
	this->gpioEn = &gpioEn;
	assigneStat = MOTOR_ASSIGNE_GPIO;
}

void MOTOR::duty(float motorDuty){
	switch(assigneStat){
	case MOTOR_ASSIGNE_PWM:
		pwmEn->duty(pwmEn->pwm_period);
		break;
	case MOTOR_ASSIGNE_GPIO:
		gpioEn->write(Bit_SET);
		break;
	}


	int16_t motorDuty16 = (int16_t)(motorDuty * pwm1->pwm_period);

	uint16_t dutyMax = (pwm1->pwm_period) - 50;	//スイッチング時間分でduty上限かける
	if(motorDuty16 >  dutyMax)motorDuty16 = dutyMax;
	if(motorDuty16 < -dutyMax)motorDuty16 = -dutyMax;

	if(motorDuty16 > 0){
		pwm1->duty(0);
		pwm2->duty(motorDuty16);
	}else{
		pwm2->duty(0);
		pwm1->duty(-motorDuty16);
	}
	outDuty = motorDuty16;

}

void MOTOR::brake(float motorBrake){
	pwm1->duty(0);
	pwm2->duty(0);
	pwmEn->duty(pwmEn->pwm_period * motorBrake);
}

void MOTOR::lock(){
	pwm1->duty(0);
	pwm2->duty(0);
	if(assigneStat == MOTOR_ASSIGNE_PWM){
		pwmEn->duty(pwmEn->pwm_period);
	}else if(assigneStat == MOTOR_ASSIGNE_GPIO){
		gpioEn->write(Bit_SET);
	}
}

void MOTOR::free(){
	pwm1->duty(0);
	pwm2->duty(0);
	if(assigneStat == MOTOR_ASSIGNE_PWM){
		pwmEn->duty(0);
	}else if(assigneStat == MOTOR_ASSIGNE_GPIO){
		gpioEn->write(Bit_RESET);
	}
}
