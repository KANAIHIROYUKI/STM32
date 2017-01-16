#ifndef _PORT_H_
#define _PORT_H_

#include "gpio.h"

class PORT:public GPIO{
public:
	GPIO io[16];
	void write(uint16_t value);
	uint16_t read();
	void invert();
	uint16_t port_num;
};


#endif
