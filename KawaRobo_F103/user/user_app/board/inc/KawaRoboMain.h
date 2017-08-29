#ifndef _KRM_H_
#define _KRM_H_

#include "system.h"
#include "app.h"

#define KRM_SBUS_ALD_READ 49

#define KRM_CHANNEL_REV  3
#define KRM_CHANNEL_RUN  1

#define KRM_CHANNEL_MODE 2
#define KRM_CHANNEL_ARM  6


#define KRM_MODE_RUN  2
#define KRM_MODE_TEST 1
#define KRM_MODE_STOP 0

#define KRM_MODE_TIMEOUT 3

#define KRM_PRINT_CYCLE 100
#define KRM_TIMEOUT_TOLERANCE 100


class KawaRobo {
public:
	void setup(USART &serial,SBUS &sbus,SerialArduino &sa,DutyOut &motor0,DutyOut &motor1,DutyOut &motor2,DutyOut &motor3);
	void uiSetup(Switch &sw0,Switch sw1,GPIO &led0,GPIO &led1,GPIO &led2,GPIO &led3);

	void cycle();
	void controlCycle();
	void run();

	int16_t forward,revolution,armPower;
	int16_t gyroSpeed;

	uint16_t mode,printValueSelect;
	uint64_t printTime;

private:
	USART *serial;
	DutyOut *motor[4];
	SerialArduino *sa;
	SBUS *sbus;

	GPIO *led[4];
	Switch *sw[2];

	TIM enc;
	AS504x mgEnc;
};


#endif
