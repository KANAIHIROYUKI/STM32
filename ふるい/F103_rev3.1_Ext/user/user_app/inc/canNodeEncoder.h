#ifndef _CAN_NODE_ENCODER_
#define _CAN_NODE_ENCODER_

#include "stm32f10x.h"
#include "stdio.h"

#include "system.h"

class CanNodeEncoder{
public:
	int16_t setup(TIM &enc,CAN &can,uint16_t address);
	void cycle();
	void interrupt(CanRxMsg rxMessage);

	uint32_t canEnc_interval;
	TIM* canEnc_enc;

private:
	CAN* canEnc_can;
	uint32_t canEnc_intervalTimer;
	uint16_t canEnc_address;

};



#endif
