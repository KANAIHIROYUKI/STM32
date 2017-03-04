#ifndef CAN_MD_H_
#define CAN_MD_H_

#include "system.h"

#include "motor.h"

#define LED_INTERVAL_NORECEIVE 50
#define LED_INTERVAL_RECEIVE 500

#define CAN_MD_ADDRESS_DUTY 0
#define CAN_MD_ADDRESS_FREE 1

class CanNodeMotorDriver{
public:
	int16_t setup(Motor &motor,CAN &can,uint16_t number,uint16_t maxInterval = 100);
	void ledAssign(GPIO &led);
	void cycle();
	void interrupt();

	int32_t outDuty;
	float outDutyF;
	uint16_t canMd_address[2];

	GPIO *canMd_led;
	Motor* canMd_motor;

	uint8_t ledAssigned;

	uint64_t canMd_receiveTime,canMd_ledTime;
	uint16_t canMd_maxInterval;

private:
	CAN* canMd_can;
};



#endif
