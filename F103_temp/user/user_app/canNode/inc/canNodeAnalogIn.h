#ifndef CAN_NODE_ANALOG_H_
#define CAN_NODE_ANALOG_H_

#include "system.h"
#include "util.h"

class CanNodeAnalog{
public:
	int16_t setup(ADC &adc,CAN &can,uint16_t number);
	void cycle();
	void interrupt();

	ADC *adc;
	uint16_t address;

	uint32_t interval;
	uint32_t intervalTimer;
private:
	CAN *can;

};


#endif
