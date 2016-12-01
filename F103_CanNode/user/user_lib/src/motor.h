#ifndef MOTOR_H_
#define MOTOR_H_

#include "stm32f10x.h"
#include "stdio.h"
#include "math.h"

#include "system.h"

class MOTOR{
public:
	TIM *PWM1;
	TIM *PWM2;
	void setup(TIM &PWM_1,TIM &PWM_2t);
	void test();
	void duty(int32_t motor_duty);

private:
};


#endif
