#ifndef CANMD_H_
#define CANMD_H_

#include "system.h"
#include "math.h"

class CanMotorDriver{
public:
	void setup(CAN &can,uint16_t number);
	void duty(int16_t motor_duty);

	uint16_t canMotorDriver_address;
private:
	CAN *canMotorDriver_can;
};

#endif
