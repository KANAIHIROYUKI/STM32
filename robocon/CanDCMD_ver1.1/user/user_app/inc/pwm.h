#ifndef _PWM_H_
#define _PWM_H_

#include "system.h"
#include "base.h"

class Pwm:public DutyOut{
public:
	void setup(TIM_TypeDef* tim,uint16_t channel,GPIO_TypeDef* gpio,uint16_t pin,uint16_t period = PWM_PERIOD_DEFALUT,uint16_t prescaler = 0,uint16_t mode = TIM_OCMode_DEFAULT);
	virtual void duty(float duty);
	virtual void reset();
	virtual void invert(int8_t mode);

	TIM *pwm_tim;
};

#endif
