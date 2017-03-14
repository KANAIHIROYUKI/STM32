#include "bldc.h"

void Phase::setup(TIM &tim,GPIO &en){
	this->tim = &tim;
	this->en = &en;
}

void Phase::adcAssigne(ADC &adc){
	this->adc = &adc;
}

void Phase::zeroCrossAssigne(GPIO &zero){
	this->zc = &zero;
}

void Phase::degree(int16_t out_degree){
	out_degree = out_degree%360;
	if(out_degree < 0)out_degree = 360 + out_degree;

	if(out_degree <= 60){
		duty(out_degree/60);
	}else if(out_degree <= 180){
		duty(1.0);
	}else if(out_degree <= 240){
		duty(1.0 - (180 - out_degree)/60);
	}else{
		duty(0);
	}
}

void Phase::duty(float outDuty){
	if(outDuty <= 0.95){
		tim->duty(outDuty * tim->pwm_period);
	}else if(outDuty <= 0.05){
		tim->duty(0);
	}else{
		tim->duty(tim->pwm_period);
	}
}

/***************************************************************/

void BLDC::setup(Phase &U,Phase &V,Phase &W){
	this->phase[0] = &U;
	this->phase[1] = &V;
	this->phase[2] = &W;
}


void BLDC::degree(int16_t out_degree){
	phase[0]->degree(out_degree);
	phase[1]->degree(out_degree + 120);
	phase[2]->degree(out_degree + 240);
}



