#ifndef _CAN_NODE_VALVE_
#define _CAN_NODE_VALVE_

#include "system.h"
#include "gpio.h"

class CanNodeValve{
public:
	int setup(CAN &can,uint16_t number);
	void pinAdd(GPIO &gpio);
	void interrupt();

	GPIO *gpio[10];

	uint16_t address;
	uint8_t pinNumber;

private:
	CAN *can;
};


#endif
