#ifndef CANMD_H_
#define CANMD_H_

#include "system.h"
#include "math.h"

class CanMotor{
public:
	void setup(CAN &can,uint16_t number);
	void duty(int16_t motor_duty);
private:
	CAN *canMotor_can;
};

#endif
