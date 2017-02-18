#ifndef _CAN_NODE_VALVE_
#define _CAN_NODE_VALVE_

#include "system.h"
#include "port.h"

class CanNodeValve{
public:
	int setup(GPIO &gpio,uint8_t pinNumber,CAN &can,uint16_t address);
	void interrupt(CanRxMsg rxMessage);
private:
	GPIO *canValve_gpio;
	CAN *canValve_can;
	uint16_t canValve_address;
};


#endif
