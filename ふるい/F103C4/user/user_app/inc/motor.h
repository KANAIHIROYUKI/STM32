#ifndef MOTOR_H_
#define MOTOR_H_

#include "math.h"

#include "system.h"

class MOTOR{
public:
	TIM *PWM1;
	TIM *PWM2;
	void setup(TIM &PWM_1,TIM &PWM_2t);
	void duty(int32_t motor_duty);

	int32_t outDuty;
private:
};


#endif
