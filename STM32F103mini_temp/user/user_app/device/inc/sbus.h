#ifndef _SBUS_H_
#define _SBUS_H_

#include "system.h"
#include "usart.h"

#define SBUS_START_BYTE 0x0F
#define SBUS_END_BYTE   0x00

#define SBUS_BUFFER_END 25


#define SBUS_ERROR_NONE		0x00
#define SBUS_ERROR_FRAME	0x10

class SBUS{
public:
	void setup(USART &usart);

	float readF(uint16_t channel);
	uint16_t read(uint16_t channel);
	void cycle();

	uint64_t lastReceiveTime,receiveIntervalTime;
	uint16_t receiveBytes,channel[18],errorValue,update;
	uint8_t buffer[51];
	USART *serial;
};








#endif
