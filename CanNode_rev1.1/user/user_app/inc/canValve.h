#ifndef CAN_VALVE_H_
#define CAN_VALVE_H_

#include "system.h"

class CanValve{
public:
	void setup(CAN &can,uint16_t number);
	void write(uint8_t pin,uint8_t stat);
private:
	CAN *canValve_can;
	uint16_t canValve_address;
};


#endif
