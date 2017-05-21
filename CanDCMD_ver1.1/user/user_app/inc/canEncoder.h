#ifndef CANENCODER_H_
#define CANENCODER_H_

#include "system.h"
#include "util.h"
#include "base.h"

class CanEncoder{
public :
	void setup(CAN &can,uint16_t number,uint16_t interval);
	int32_t read();
	void reset();
	void interrupt();
	void reverse(int8_t wise = -1);
	uint64_t lastReceiveTime();
	uint64_t canEnc_receiveTime;
	int32_t canEnc_value;
	uint16_t canEnc_address;
	int8_t canEnc_wise;

private:
	CAN *canEnc_can;

};

#endif
