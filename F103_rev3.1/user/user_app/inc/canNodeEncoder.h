#ifndef _CAN_NODE_ENCODER_
#define _CAN_NODE_ENCODER_

#include "system.h"
#include "util.h"

class CanNodeEncoder{
public:
	int16_t setup(TIM &enc,CAN &can,uint16_t number);
	void cycle();
	void interrupt(CanRxMsg rxMessage);

	uint32_t canEnc_interval;
private:
	CAN* canEnc_can;
	TIM* canEnc_enc;
	uint32_t canEnc_intervalTimer;
	uint16_t canEnc_address;

};



#endif
