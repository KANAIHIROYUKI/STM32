#ifndef _CAN_NODE_VOLTAGE_H_
#define _CAN_NODE_VOLTAGE_H_

#include "si8900.h"
#include "system.h"
#include "can.h"
#include "util.h"
#include "util.hpp"

class CanNodeVoltage{
public:
	int16_t setup(SI8900 &si8900set,uint16_t portSet,float gainSet,CAN &can,uint16_t numberSet);
	void cycle();
	void interrupt();

	float gain,voltageValue;
	uint32_t interval;
	SI8900 *si8900;
	uint32_t intervalTimer;
	uint16_t number,port;

	Average<float> vAve;
private:
	CAN* can;
};



#endif
