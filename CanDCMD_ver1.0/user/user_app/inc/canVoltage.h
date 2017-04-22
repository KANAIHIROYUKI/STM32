#ifndef _CAN_VOLTAGE_H_
#define _CAN_VOLTAGE_H_

#include "system.h"
#include "can.h"
#include "util.h"

class CanVoltage{
public :
	void setup(CAN &can,uint16_t number,uint16_t interval);
	int32_t read();
	void interrupt();

	uint64_t lastReceiveTime();
	uint64_t receiveTime;
	int32_t value;
	uint16_t number;

private:
	CAN *can;
};

#endif
