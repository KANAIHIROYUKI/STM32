#ifndef MAIN_H_
#define MAIN_H_

#include "system.h"
#include "app.h"
#include "util.h"

System sys;

USART serial;
TIM pwm[2];
GPIO gdEn,kickEn,kick[2];
ADC vFB,vSet;
Switch sw[3];
LED led;

uint64_t intervalTime = 0,controlCycleTime = 0;

void setup(){
	sys.setup();
	serial.setup(USART1_Setup,115200);
	sys.usartSetup(serial);

	pwm[0].pwmSetup(TIM2_PWM1_Setup,200,TIM_OCMode_PWM1);
	pwm[1].pwmSetup(TIM2_PWM2_Setup,200,TIM_OCMode_PWM2);
	gdEn.setup(PA2,OUTPUT);

	kickEn.setup(PC15,OUTPUT);
	kick[0].setup(PA6,OUTPUT);
	kick[1].setup(PA7,OUTPUT);

	sw[0].setup(PB0);
	sw[1].setup(PB1);
	sw[2].setup(PB2);

	vFB.setup(ADC13_Setup);
	vSet.setup(ADC24_Setup);

	led.setup(PC14);
	led.interval(500);


	pwm[0].duty(100);
	pwm[1].duty(100);

	kickEn.write(1);
}



#endif
