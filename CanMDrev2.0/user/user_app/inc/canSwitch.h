#ifndef _CAN_SW_H_
#define _CAN_SW_H_

#include "can.h"
#include "switch.h"
#include "system.h"

class CanSwitch{
public:
	void setup(CAN &can_,uint16_t number,uint32_t interval);
	uint16_t read();
	void interrupt();

	uint16_t canAddress,switchValue;
	uint64_t lastReceiveTime;
	CAN *can;
};

#endif
