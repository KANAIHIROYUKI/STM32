#ifndef _CANNODE_SW_H_
#define _CANNODE_SW_H_

#include "system.h"
#include "can.h"
#include "app.h"

class CanNodeSwitch{
public:
	void setup(Switch &sw,CAN &can_,uint16_t canNumber);
	void cycle();
	void interrupt();

	uint8_t canData[4];
	uint16_t canAddress;
	uint16_t setuped;
	uint32_t intervalTime;
	uint64_t lastSendTime;
	Switch *sw;
	CAN *can;
};

#endif
