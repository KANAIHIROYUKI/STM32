#ifndef MOTOR_H_
#define MOTOR_H_

#include "math.h"
#include "base.h"
#include "system.h"

#define MOTOR_ASSIGNE_NONE 0
#define MOTOR_ASSIGNE_PWM 1
#define MOTOR_ASSIGNE_GPIO 2

#define MOTOR_ENABLE_DISABLE 0
#define MOTOR_ENABLE_ENABLE  1
#define MOTOR_ENABLE_BUZZER  2

#define FRQ_THR		0.9
#define LOW_FRQ_PRI 35000	//2kHz

class Motor{
public:

	void setup(TIM &pwm1,TIM &pwm2);
	void enPwmAssigne(TIM &pwmEN);
	void enPinAssigne(GPIO &gpioEn);
	void duty(float motorDuty);
	void brake(float motorBrake);

	void lock();
	void free();

	void reset();

	void buzzerStart(uint16_t frequency,float duty);
	void buzzerStop();

	int32_t outDuty;
	uint16_t assigneStat,outEnable;

	uint16_t pwm_period,pwm_prescaler,pwm_mode;
	uint16_t en_period,en_prescaler,en_mode;

	uint16_t defaultFrq,pwmFrqStat;


	TIM *pwm1;
	TIM *pwm2;
	TIM *pwmEn;
	GPIO *gpioEn;
private:
};


#endif
