#ifndef CAN_PULSE_H_
#define CAN_PULSE_H_

#include "system.h"

#define AMP_PULSE_CENTER 1500
#define AMP_PULSE_MAX 2200
#define AMP_PULSE_MIN 800

class CanNodePulse{
public:
	int16_t setup(TIM &tim,CAN &can,uint16_t number);
	void interrupt();

	int16_t outDuty;
	uint16_t canPulse_address;
	TIM *canPulse_tim;
private:
	CAN *canPulse_can;
};



#endif
