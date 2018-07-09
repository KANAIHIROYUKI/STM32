#ifndef CAN_PULSE_H_
#define CAN_PULSE_H_

#include "system.h"
#include "base.h"
#include "gpio.h"

#define AMP_PULSE_CENTER 1500
#define AMP_PULSE_MAX 2200
#define AMP_PULSE_MIN 800

#define AMP_PULSE_CYCLE 20000

#define CAN_PULSE_MODE_TIM 0
#define CAN_PULSE_MODE_IO 1

class CanNodePulse{
public:
	int16_t setup(TIM &tim,CAN &can,uint16_t number);
	int16_t setup(GPIO &gpio,CAN &can,uint16_t number);
	void interrupt();
	void cycle();
	void invert();

	int16_t outDuty;
	uint16_t address;
	uint16_t mode;

	uint16_t pulseDuty;
	uint64_t pulseTime;	//パルス幅タイマ
	uint64_t cycleTime;	//パルス周期タイマ

	TIM *tim;
	GPIO *gpio;
private:
	CAN *can;
};



#endif
