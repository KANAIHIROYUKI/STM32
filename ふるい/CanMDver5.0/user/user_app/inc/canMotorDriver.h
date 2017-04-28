#ifndef CANMD_H_
#define CANMD_H_

#include "system.h"
#include "math.h"

#define CAN_MD_ADDRESS_DUTY 0
#define CAN_MD_ADDRESS_FREE 1

class CanMotorDriver{
public:
	void setup(CAN &can,uint16_t number);
	void duty(float motor_duty);
	void brake(float motor_brake);

	uint16_t canMotorDriver_address[2];
	int16_t outDuty16;
private:
	CAN *canMotorDriver_can;
};

#endif
