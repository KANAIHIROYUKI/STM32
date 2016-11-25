#ifndef APP_H_
#define APP_H_

#include "stm32f10x.h"
#include "stdio.h"
#include "math.h"


#include "system.h"

class MOTOR{
public:
	TIM *PWM1;
	TIM *PWM2;
	uint32_t outDuty = 0;
	void setup(TIM &PWM_1,TIM &PWM_2t);
	void test();
	void duty(int32_t motor_duty);

private:
};


#endif
