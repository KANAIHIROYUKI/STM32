#ifndef _SI8900_H_
#define _SI8900_H_

#include "usart.h"
#include "system.h"

#define SI8900_MODE_DEMAND 0
#define SI8900_MODE_BURST 1
#define SI8900_MODE_LOOP 2

class SI8900{
public:
	void setup(USART &usart,uint16_t mode = SI8900_MODE_BURST);
	void request(uint16_t channel = 0,uint16_t convert_mode = SI8900_MODE_DEMAND);
	void cycle();
	uint16_t read(uint16_t channel);
	uint16_t stat(uint16_t channel);

	uint16_t timingCalibration(uint64_t timeOut = 500);

	uint16_t buffer,value[3],mxAddress,mode,readStat[3],mxCount,setupStat,requestChannel,setupData;
	uint64_t receiveTime[3],setupLastSendTime,requestTime;
	USART *usart;
};

#endif
