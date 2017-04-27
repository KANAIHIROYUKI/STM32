#include "motor.h"

void Motor::setup(TIM &PWM_1,TIM &PWM_2){
	this->pwm1 = &PWM_1;
	this->pwm2 = &PWM_2;

	outEnable = 1;

	free();

	assigneStat = MOTOR_ASSIGNE_NONE;
}
void Motor::enPwmAssigne(TIM &PWM_EN){
	this->pwmEn = &PWM_EN;
	assigneStat = MOTOR_ASSIGNE_PWM;
}

void Motor::enPinAssigne(GPIO &gpioEn){
	this->gpioEn = &gpioEn;
	assigneStat = MOTOR_ASSIGNE_GPIO;
}

void Motor::duty(float motorDuty){
	if(outEnable == 0){
		free();
		return;
	}

	switch(assigneStat){
	case MOTOR_ASSIGNE_NONE:

		break;
	case MOTOR_ASSIGNE_PWM:
		pwmEn->dutyF(1.0);
		break;
	case MOTOR_ASSIGNE_GPIO:
		gpioEn->write(Bit_SET);
		break;
	}

	int16_t motorDuty16 = (int16_t)(motorDuty * pwm1->pwm_period);

	uint16_t dutyLimit = (pwm1->pwm_period) - 50;	//�X�C�b�`���O���ԕ���duty���������
	if(motorDuty16 >  dutyLimit)motorDuty16 = dutyLimit;
	if(motorDuty16 < -dutyLimit)motorDuty16 = -dutyLimit;
	//if(motorDuty16 < 50 && motorDuty16 > -50)motorDuty16 = 0;


	pwm1->duty(pwm1->pwm_period/2 - motorDuty16/2);
	pwm2->duty(pwm2->pwm_period/2 + motorDuty16/2);
	outDuty = motorDuty16;

}

void Motor::brake(float motorBrake){
	if(outEnable == 0){
		free();
		return;
	}

	pwm1->duty(0);
	pwm2->duty(0);
	pwmEn->dutyF(motorBrake);
}

void Motor::lock(){
	if(outEnable == 0){
		free();
		return;
	}

	pwm1->duty(0);
	pwm2->duty(0);
	if(assigneStat == MOTOR_ASSIGNE_PWM){
		pwmEn->dutyF(1.0);
	}else if(assigneStat == MOTOR_ASSIGNE_GPIO){
		gpioEn->write(Bit_SET);
	}
}

void Motor::free(){

	pwm1->duty(0);
	pwm2->duty(0);

	switch(assigneStat){
	case MOTOR_ASSIGNE_NONE:

		break;
	case MOTOR_ASSIGNE_PWM:
		pwmEn->duty(0);
		break;
	case MOTOR_ASSIGNE_GPIO:
		gpioEn->write(Bit_RESET);
		break;
	}
}

void Motor::reset(){
	free();
}