#ifndef _CAN_NODE_VALVE_
#define _CAN_NODE_VALVE_

#include "system.h"
#include "port.h"

class CanNodeValve{
public:
	int setup(PORT &port,CAN &can,uint16_t address);
	//void cycle();
	void interrupt(CanRxMsg rxMessage);
private:
	PORT *canValve_port;
	CAN *canValve_can;
	uint16_t canValve_address;
};


#endif
