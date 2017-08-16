#include "si8274.h"

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

	uint16_t dutyLimit = (pwm1->pwm_period) - 50;	//スイッチング時間分でduty上限かける
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

void Motor::buzzerStart(uint16_t frequency,float duty){
	if(assigneStat != MOTOR_ASSIGNE_PWM)	return;		//PWMセットされてないからビープ使えない
	if(outEnable == 0)						return;		//出力無効

	if(outEnable == 1){
		en_period 		= pwmEn->pwm_period;
		en_prescaler 	= pwmEn->pwm_prescaler;
		en_mode 		= pwmEn->pwm_mode;

		pwm_period 		= pwm1->pwm_period;
		pwm_prescaler 	= pwm1->pwm_prescaler;
		pwm_mode 		= pwm1->pwm_mode;
	}

	outEnable = 2;										//2がビープ

	frequency = 72000000/frequency;

	pwm1->pwmReset(frequency,0,TIM_OCMode_PWM1);
	pwm2->pwmReset(frequency,0,TIM_OCMode_PWM2);
	pwmEn->pwmReset(frequency/2,0,TIM_OCMode_PWM1);

	pwm1->duty(frequency/2);
	pwm2->duty(frequency/2);
	//pwmEn->duty((uint32_t)((float)frequency*duty));	//ここらへんおかしい､dutyFつかえん
	pwmEn->duty((frequency/2)*duty);							// 1/4
	//pwmEn->dutyF(0.25);
}

void Motor::buzzerStop(){
	if(outEnable != 2)return;

	outEnable = 1;

	pwm1->pwmReset(pwm_period,pwm_prescaler,pwm_mode);
	pwm2->pwmReset(pwm_period,pwm_prescaler,pwm_mode);
	pwmEn->pwmReset(en_period,en_period,en_mode);

	pwm1->duty(0);
	pwm2->duty(0);
	pwmEn->duty(0);
}
