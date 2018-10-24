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


	for(int i=0;i<4;i++){
		errorMin[i] = 0;
		errorMax[i] = 0;
		errorLimitSetuped[i] = 0;
	}//*/
}

void PID::gainSet(float KpSet,float KiSet,float KdSet){
	Kp = KpSet;
	Ki = KiSet;
	Kd = KdSet;
}

void PID::outLimit(float minSet,float maxSet){
	min = minSet;
	max = maxSet;
	mmSetuped = 1;
}

void PID::clear(){
	error = 0;
	errorOld = 0;
	errorInt = 0;
}

void PID::feedforwardOffset(float _errorBorder,float _static,float _speedBorder,float _dynamic){
	errorBorder = _errorBorder;
	speedBorder = _speedBorder;
	offsetStatic = _static;
	offsetDynamic = _dynamic;
}


void PID::errorLimitP(float min,float max){
	errorLimit(PID_P,min,max);
}
void PID::errorLimitI(float min,float max){
	errorLimit(PID_I,min,max);
}
void PID::errorLimitD(float min,float max){
	errorLimit(PID_D,min,max);
}

void PID::errorLimitInt(float min,float max){
	errorLimit(PID_Int,min,max);
}


void PID::errorLimit(uint16_t PorIorD,float min,float max){
	if(PorIorD > 4)return;
	errorLimitSetuped[PorIorD] = 1;
	errorMin[PorIorD] = min;
	errorMax[PorIorD] = max;
}
//*/

//���݂̖ڕW
void PID::setPoint(float setpoint){
	sp = setpoint;

	error = sp - pv;
	errorInt += sp - pv;

	if(errorLimitSetuped[PID_Int])errorInt = floatlimit(errorMax[PID_Int],errorInt,errorMin[PID_Int]);
}

//���݂̒l
void PID::measuredValue(float measuredvalue){
	pv = measuredvalue;

	error = measuredvalue;
	errorDiv = valOld - error;
	errorInt += error;
	valOld = pv;

	if(errorLimitSetuped[PID_Int])errorInt = floatlimit(errorMax[PID_Int],errorInt,errorMin[PID_Int]);
}

//���͂Əo�͂̍�(e)��^����
void PID::input(float in){
	errorOld = error;
	error = in;
	errorDiv = errorOld - error;
	errorInt += error;
	if(errorLimitSetuped[PID_Int])errorInt = floatlimit(errorMax[PID_Int],errorInt,errorMin[PID_Int]);
}

void PID::input(float set,float val){
	error = set - val;
	errorDiv = valOld - val;
	errorInt += error;
	valOld = val;

	if(errorLimitSetuped[PID_Int])errorInt = floatlimit(errorMax[PID_Int],errorInt,errorMin[PID_Int]);
}

float PID::outputF(){
	float outPID[3],out = 0;

	outPID[PID_P] = error * Kp;
	outPID[PID_I] = errorInt * Ki;
	outPID[PID_D] = errorDiv * Kd;

	for(int i=0;i<3;i++){
		if(errorLimitSetuped[i])outPID[i] = floatlimit(errorMax[i],outPID[i],errorMin[i]);
		out += outPID[i];
	}

	if(errorDiv > speedBorder || errorDiv < -speedBorder){
		if(error > 0){
			out += offsetDynamic;
		}else if(error < 0){
			out -= offsetDynamic;
		}
	}else{
		if(error > errorBorder){
			out += offsetStatic;
		}else if(error < -errorBorder){
			out -= offsetStatic;
		}else{

		}
	}


	if(mmSetuped)out = floatlimit(max,out,min);
	return out;
}

int32_t PID::output32(){
	return (int32_t)(outputF() * 100);
}