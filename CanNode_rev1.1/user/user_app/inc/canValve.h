#ifndef CAN_VALVE_H_
#define CAN_VALVE_H_

#include "system.h"
#include "base.h"

class CanValve{
public:
	void setup(CAN &can,uint16_t number);
	void write(uint8_t pin,uint8_t stat);

	uint16_t canValve_address;
private:
	CAN *canValve_can;
};


#endif
