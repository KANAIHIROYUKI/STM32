#ifndef _CANNODE_EMG_H_
#define _CANNODE_EMG_H_

#include "system.h"
#include "can.h"
#include "app.h"

class CanNodeEmg{
public:
	void setup(CAN &canSet,uint16_t myAddress = 0);
	void emgRequest(uint16_t emgMessage = 0);
	uint16_t emgRead();

	void interrupt();

	uint8_t data[8];
	uint16_t emgStat,address;
	CAN *can;
};


#endif
