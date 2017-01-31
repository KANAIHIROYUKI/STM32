#ifndef _CAN_NODE_VALVE_
#define _CAN_NODE_VALVE_

#include "system.h"
#include "gpio.h"

class CanNodeValve{
public:
	int setup(CAN &can,uint16_t number);
	void pinAdd(GPIO &gpio);
	void interrupt();

	GPIO *canValve_gpio[10];

	uint16_t canValve_address;
	uint8_t canValuve_pinNumber;

private:
	CAN *canValve_can;
};


#endif
