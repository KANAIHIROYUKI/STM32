#ifndef CAN_MD_H_
#define CAN_MD_H_

#include "system.h"

#include "si8274.h"

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

	void motorOverRide(uint16_t overRideEn);
	void ledOverRide(uint16_t overRideEn);

	int32_t outDuty;
	float outDutyF;
	uint16_t address[2];

	GPIO *led;
	Motor* motor;

	uint8_t ledAssigned,motorOutEnable;

	uint64_t lastReceiveTime,ledTime;
	uint16_t maxInterval;

private:
	CAN* can;
};



#endif

