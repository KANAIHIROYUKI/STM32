#ifndef CANENCODER_H_
#define CANENCODER_H_

#include "system.h"
#include "util.h"

class CanEncoder{
public :
	void setup(CAN &can,uint16_t number,uint16_t interval);
	uint32_t read();
	void reset();
	void interrupt(CanRxMsg rxMessage);
	uint64_t lastReceiveTime();
	uint64_t canEnc_receiveTime;

private:

	CAN *canEnc_can;
	uint16_t canEnc_address;
	int32_t canEnc_value;
};

#endif
