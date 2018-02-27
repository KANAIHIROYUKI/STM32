#ifndef _NCP5359_H_
#define _NCP5359_H_

#include "system.h"
#include "base.h"
#include "tim.h"
#include "gpio.h"
#include "util.h"
#include "math.h"

#define TIME_OUT 100
#define THR_DUTY 0.9

class NCP5359 :public DutyOut{
public:
	void setup(TIM &pwm1Set,TIM &pwm2Set);
	void cycle();
	void duty(float duty);
	void dutyLimit(float duty);
	void dutyLimit(float dutyp,float dutym);

	int outFrq;
	float outDuty,limitp,limitm;
	uint64_t dutySetTime;
	TIM *pwm[2];
private:
};

/*
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

*/
#endif
