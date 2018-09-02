#ifndef CANMD_H_
#define CANMD_H_

#include "system.h"
#include "math.h"

class CanMotorDriver{
public:
	void setup(CAN &can,uint16_t number);
	void duty(float motor_duty);
	void free();
	void brake(float motor_brake);

	void position(float pos);
	void velocity();
	void setting();
	void debug();

	uint16_t address;
	int16_t outDuty16,outFree16;
	float outPosition;
private:
	CAN *can;
};

#endif
