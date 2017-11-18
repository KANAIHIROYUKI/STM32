#ifndef _PID_CTR_H_
#define _PID_CTR_H_

#define PID_P 0
#define PID_I 1
#define PID_D 2

#define PID_Int 3

#include "system.h"
#include "util.h"

class PID{
public:
	void setup(float Kp,float Ki,float Kd);
	void gainSet(float Kp,float Ki,float Kd);
	void outLimit(float min,float max);
	void clear();

	void errorLimitP(float min,float max);
	void errorLimitI(float min,float max);
	void errorLimitD(float min,float max);//出力時の各ゲイン上限

	void errorLimitInt(float min,float max);//Iの積分値上限

	void setPoint(float setpoint);
	void measuredValue(float measuredvalue);

	void input(float in);

	float outputF();
	int32_t output32();

	float Kp,Ki,Kd,pv,sp,min,max,errorMin[4],errorMax[4],errorLimitSetuped[4];
	float error,errorOld,errorInt,errorDiv;
	uint16_t mmSetuped;

private:
	void errorLimit(uint16_t PorIorD,float min,float max);
};

#endif
