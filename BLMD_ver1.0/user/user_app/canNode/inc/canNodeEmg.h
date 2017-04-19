#ifndef _CANNODE_EMG_H_
#define _CANNODE_EMG_H_

#include "can.h"

class CanNodeEmg{
public:
	void setup(CAN &canSet);
	void emgRequest();

	void interrupt();

	uint8_t data[8];
	uint16_t emgStat;
	CAN *can;
};


#endif
