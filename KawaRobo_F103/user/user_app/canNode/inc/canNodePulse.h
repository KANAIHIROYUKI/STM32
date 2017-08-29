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
	uint16_t canPulse_address;
	uint16_t canPulse_mode;

	uint16_t canPulse_pulseDuty;
	uint64_t canPulse_pulseTime;	//パルス幅タイマ
	uint64_t canPulse_cycleTime;	//パルス周期タイマ

	TIM *canPulse_tim;
	GPIO *canPulse_gpio;
private:
	CAN *canPulse_can;
};



#endif
