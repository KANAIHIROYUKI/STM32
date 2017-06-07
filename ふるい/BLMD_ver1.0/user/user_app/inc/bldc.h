#ifndef _BLDC_H_
#define _BLDC_H_

#include "system.h"

class Phase{
public:
	void setup(TIM &tim,GPIO &en);
	void adcAssigne(ADC &adc);
	void zeroCrossAssigne(GPIO &zero);
	void degree(int16_t out_degree,float duty);

	void duty(float outDuty);
	TIM *tim;
	GPIO *en;
	GPIO *zc;
	ADC *adc;
};

class BLDC{
public:
	void setup(Phase &U,Phase &V,Phase &W);
	void degree(int16_t out_degree,float duty);
	void lock();
	void free();
	Phase* phase[3];
};

#endif
