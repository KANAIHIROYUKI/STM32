#ifndef _PID_CTR_H_
#define _PID_CTR_H_


#include "system.h"

class PID{
public:
	void setup(float Kp,float Ki,float Kd);
	void gainSet(float Kp,float Ki,float Kd);
	void limit(float min,float max);

	void setPoint(float setpoint);
	void measuredValue(float measuredvalue);

	void input(float in);

	float outputF();
	int32_t output32();

	float Kp,Ki,Kd,pv,sp,min,max;
	float error,errorOld,errorInt,errorDiv;
	uint16_t mmSetuped;
};

#endif
