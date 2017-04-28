#ifndef _BLDC_H_
#define _BLDC_H_

#include "system.h"

class Phase{
public:
	void setup(TIM &tim,GPIO &en);
	void adcAssigne(ADC &adc);
	void zeroCrossAssigne(GPIO &zero);
	void degree(int16_t out_degree);

	void duty(float outDuty);
	TIM *tim;
	GPIO *en;
	GPIO *zc;
	ADC *adc;
};

class BLDC{
public:
	void setup(Phase &U,Phase &V,Phase &W);
	void degree(int16_t out_degree);
	Phase* phase[3];
};

#endif
