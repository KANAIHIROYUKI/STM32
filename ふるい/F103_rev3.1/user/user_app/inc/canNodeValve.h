#ifndef _CAN_NODE_VALVE_
#define _CAN_NODE_VALVE_

#include "system.h"
#include "gpio.h"

class CanNodeValve{
public:
	int setup(CAN &can,uint16_t address);
	//void cycle();
	void pinAdd(GPIO &gpio);
	void interrupt(CanRxMsg rxMessage);
private:
	GPIO *canValve_gpio[8];
	CAN *canValve_can;
	uint16_t canValve_address;
	uint8_t canValuve_pinNumber;
};


#endif
