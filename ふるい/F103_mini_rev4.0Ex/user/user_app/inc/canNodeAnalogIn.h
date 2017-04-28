#ifndef CAN_NODE_ANALOG_H_
#define CAN_NODE_ANALOG_H_

#include "system.h"
#include "util.h"

class CanNodeAnalog{
public:
	int16_t setup(ADC &adc,CAN &can,uint16_t number);
	void cycle();
	void interrupt();

	ADC *canNodeAnalog_adc;
	uint16_t canNodeAnalog_address;

	uint32_t canNodeAnalog_interval;
	uint32_t canNodeAnalog_intervalTimer;
private:
	CAN *canNodeAnalog_can;

};


#endif
