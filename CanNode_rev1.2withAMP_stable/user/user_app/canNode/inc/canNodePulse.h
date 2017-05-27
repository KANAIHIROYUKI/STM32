#ifndef CAN_PULSE_H_
#define CAN_PULSE_H_

#include "system.h"
#include "base.h"
#include "gpio.h"

#define AMP_PULSE_CENTER 960
#define AMP_PULSE_MAX 1960
#define AMP_PULSE_MIN 960

//#define AMP_PULSE_CYCLE 17000
#define AMP_PULSE_CYCLE 2000
#define AMP_PULSE_LIMIT 800

#define CAN_PULSE_MODE_TIM 0
#define CAN_PULSE_MODE_IO 1

class CanNodePulse{
public:
	int16_t setup(TIM &tim,CAN &can,uint16_t number);
	int16_t setup(GPIO &gpio,CAN &can,uint16_t number);
	void interrupt();
	void debug(uint16_t debugDuty);
	void cycle();
	void invert();

	int16_t outDuty;
	uint16_t canPulse_address;
	uint16_t canPulse_mode;

	uint16_t canPulse_pulseDuty;
	uint64_t canPulse_pulseTime;	//パルス幅タイマ
	uint64_t canPulse_cycleTime;	//パルス周期タイマ

	uint64_t canPulse_receiveTime;

	TIM *canPulse_tim;
	GPIO *canPulse_gpio;
private:
	CAN *canPulse_can;
};



#endif
