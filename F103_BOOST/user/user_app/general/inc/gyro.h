#ifndef _GYRO_H
#define _GYRO_H_

#include "system.h"
#include "base.h"

class Gyro:public DutyIn{
	void setup();
	void reset();
	float radPerSec();
	float radian();
	float read();
};


#endif
