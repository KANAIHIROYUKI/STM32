#ifndef _CAN_NODE_ENCODER_
#define _CAN_NODE_ENCODER_

#include "system.h"
#include "util.h"

#include "USART.h"

class CanNodeEncoder{
public:
	int16_t setup(TIM &enc,CAN &can,uint16_t number);
	void cycle();
	void interrupt();

	uint32_t interval;
	TIM* enc;
	uint32_t intervalTimer;
	uint16_t address;

private:
	CAN* can;
};



#endif
