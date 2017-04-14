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

void Phase::degree(int16_t out_degree,float out_duty){
	out_degree = out_degree%360;
	if(out_degree < 0)out_degree = 360 + out_degree;

	if(out_degree < 60){
		en->write(0);
		duty(0);
	}else if(out_degree < 180){
		en->write(1);
		duty(0.5 + (out_duty/2));
	}else if(out_degree < 240){
		en->write(0);
		duty(0);
	}else if(out_degree < 360){
		en->write(1);
		duty(0.5 - (out_duty/2));
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


void BLDC::degree(int16_t out_degree,float out_duty){
	if(out_degree < 0)return;
	phase[0]->degree(out_degree,out_duty);
	phase[1]->degree(out_degree + 120,out_duty);
	phase[2]->degree(out_degree + 240,out_duty);
}

void BLDC::lock(){
	phase[0]->en->write(1);
	phase[1]->en->write(1);
	phase[2]->en->write(1);

	phase[0]->tim->duty(0);
	phase[1]->tim->duty(0);
	phase[2]->tim->duty(0);
}

void BLDC::free(){
	phase[0]->en->write(0);
	phase[1]->en->write(0);
	phase[2]->en->write(0);

	phase[0]->tim->duty(0);
	phase[1]->tim->duty(0);
	phase[2]->tim->duty(0);
}

