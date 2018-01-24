#ifndef _DYNAMIXEL485_H_
#define _DYNAMIXEL485_H_

#include "system.h"
#include "USART.h"

class Dynamixel485 {
public:
	void setup(USART &serial,GPIO &dir);

	void cycle();

	void writeDeg(uint16_t address,int16_t degree);
	void writeDuty(uint16_t address,float duty);


private:
	GPIO *direction;
	USART *rs485;
};



#endif
