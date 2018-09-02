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
	void feedforwardOffset(float errorBorder,float _static,float speedBorder,float _dynamic);

	void errorLimitP(float min,float max);
	void errorLimitI(float min,float max);
	void errorLimitD(float min,float max);//出力時の各ゲイン上限

	void errorLimitInt(float min,float max);//Iの積分値上限

	//現在の目標
	void setPoint(float setpoint);
	//現在位置
	void measuredValue(float measuredvalue);

	//Dが全部にかかってしまうアレ
	void input(float in);

	//微分先行
	void input(float set,float val);

	//ポテンショ+エンコーダ
	void input(float set,float val,float vel);

	float outputF();
	int32_t output32();

	float Kp,Ki,Kd,pv,sp,min,max,offsetDynamic,offsetStatic,speedBorder,errorBorder,errorMin[4],errorMax[4],errorLimitSetuped[4];
	float error,errorOld,errorInt,errorDiv,valOld;
	uint16_t mmSetuped;

private:
	void errorLimit(uint16_t PorIorD,float min,float max);
};

#endif
