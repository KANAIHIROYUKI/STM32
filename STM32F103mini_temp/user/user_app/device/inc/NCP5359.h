#ifndef _NCP5359_H_
#define _NCP5359_H_

#include "system.h"
#include "base.h"
#include "tim.h"
#include "gpio.h"

#define TIME_OUT 100

class NCP5359 :public DutyOut{
public:
	void setup(TIM &pwmSet,GPIO &gpioSet);
	void cycle();
	void duty(float duty = 0);


	uint64_t dutySetTime;
	GPIO *cw;
	TIM *pwm;
private:
};
#endif
