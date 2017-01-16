#ifndef CAN_MD_H_
#define CAN_MD_H_

#include "system.h"

#include "motor.h"

#define LED_INTERVAL_NORECEIVE 50
#define LED_INTERVAL_RECEIVE 500

class CanMotorDriver{
public:
	int16_t setup(MOTOR &motor,CAN &can,uint16_t address,uint16_t maxInterval = 100);
	void ledAssign(GPIO &led);
	void cycle();
	void interrupt(CanRxMsg rxMessage);

	int32_t outDuty;
	uint16_t canMd_address;

	CAN* canMd_can;
	GPIO *canMd_led;
	MOTOR* canMd_motor;

	uint8_t ledAssigned;

	uint64_t canMd_receiveTime,canMd_ledTime;
	uint16_t canMd_maxInterval;
};



#endif
