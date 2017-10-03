#include "PID_Controller.h"

void PID::setup(float KpSet,float KiSet,float KdSet){
	Kp = KpSet;
	Ki = KiSet;
	Kd = KdSet;

	error = 0;
	errorOld = 0;
	errorInt = 0;

	min = 0;
	max = 0;
	mmSetuped = 0;
}

void PID::gainSet(float KpSet,float KiSet,float KdSet){
	Kp = KpSet;
	Ki = KiSet;
	Kd = KdSet;
}

void PID::limit(float minSet,float maxSet){
	min = minSet;
	max = maxSet;
	mmSetuped = 1;
}

void PID::setPoint(float setpoint){
	sp = setpoint;

	input(sp - pv);
}

void PID::measuredValue(float measuredvalue){
	pv = measuredvalue;

	input(sp - pv);
}

void PID::input(float in){
	errorOld = error;
	error = in;
	errorDiv = errorOld - error;
	errorInt += error;
}

float PID::outputF(){
	float out = 0;
	out += error * Kp;
	out += errorInt * Ki;
	out += errorDiv * Kd;

	if(mmSetuped){
		if(out < min) out = min;
		if(out > max) out = max;
	}
	return out;
}

int32_t PID::output32(){
	return (int32_t)(outputF() * 100);
}
