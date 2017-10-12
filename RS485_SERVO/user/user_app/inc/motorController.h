#ifndef _MotorController_H_
#define _MotorController_H_

#include "system.h"
#include "base.h"
#include "Enc.h"

#define MC_CONTROL_CYCLE 10

class MotorController {
public:
	void setup(DutyOut &motor,Enc &enc,uint16_t cprSet);
	void kpSet(float kp);
	void kdSet(float kd);
	void kiSet(float ki);

	void position(float position);
	void speed(float speed);

	void cycle();

	void controlCycle();

	Enc *enc;
	DutyOut *motor;

	uint64_t controlCycleTime;

	uint16_t cntPerRev;
	uint64_t oldEncCnt,cntPerTime;

	float targetPosition;
	float currentPosition;
	float revPerSec;

	float difDif;
	float difInt;
	float dif;

	float kp,ki,kd;

	float output;
};





#endif
