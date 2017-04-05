#ifndef _SWITCH_H_
#define _SWITCH_H_

#include "gpio.h"
#include "system.h"

class Switch{
public:
	void setup(GPIO &gpio);
	uint16_t read();
	void cycle();

	uint16_t pinStat;
	uint16_t readStat;
	uint64_t changeTime,toleranceTime;
	GPIO *gpio;
};

#endif
