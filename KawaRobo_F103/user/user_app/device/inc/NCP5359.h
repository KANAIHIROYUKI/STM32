#ifndef _NCP5359_H_
#define _NCP5359_H_

#include "system.h"
#include "base.h"
#include "tim.h"
#include "gpio.h"
#include "util.h"

#define TIME_OUT 100

class NCP5359 :public DutyOut{
public:
	void setup(TIM &pwmSet,GPIO &gpioSet);
	void cycle();
	void duty(float duty);
	void dutyLimit(float duty);
	void dutyLimit(float dutyp,float dutym);

	float outDuty,limitp,limitm;
	uint64_t dutySetTime;
	GPIO *cw;
	TIM *pwm;
private:
};
#endif
