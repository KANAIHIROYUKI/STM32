#ifndef CAN_PULSE_H_
#define CAN_PULSE_H_

#include "system.h"

#define AMP_PULSE_CENTER 1500
#define AMP_PULSE_MAX 2000
#define AMP_PULSE_MIN 1000

class CanPulse{
public:
	int16_t setup(TIM &tim,CAN &can,uint16_t number);
	void interrupt();

	uint16_t canPulse_address;
	CAN *canPulse_can;
	TIM *canPulse_tim;
};



#endif
