#ifndef MOTOR_H_
#define MOTOR_H_

#include "math.h"
#include "base.h"
#include "system.h"

#define MOTOR_ASSIGNE_NONE 0
#define MOTOR_ASSIGNE_PWM 1
#define MOTOR_ASSIGNE_GPIO 2

class Motor : public DutyOut{
public:

	void setup(TIM &pwm1,TIM &pwm2);
	void enPwmAssigne(TIM &pwmEN);
	void enPinAssigne(GPIO &gpioEn);
	virtual void duty(float motorDuty);
	void brake(float motorBrake);

	void lock();
	void free();

	virtual void reset();

	int32_t outDuty;
	uint16_t assigneStat;

	TIM *pwm1;
	TIM *pwm2;
	TIM *pwmEn;
	GPIO *gpioEn;
private:
};


#endif
