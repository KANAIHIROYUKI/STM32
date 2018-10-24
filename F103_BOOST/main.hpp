#ifndef MAIN_H_
#define MAIN_H_

#include "system.h"
#include "app.h"
#include "util.h"
#include "util.hpp"

#include "CatchRoboMain.h"
#include "CatchRoboMap.h"

#define PWM_CNT 1000

System sys;

USART serial;
ADC vin;
TIM pwm;
Average<int64_t> vave;

uint64_t printTime,fbTime,chargeTimer;

int32_t adcVal,voltage,pwmDuty,oldVoltage;
uint16_t chargeEn = 0;

void setup(){
	sys.setup();
	serial.setup(USART1_Setup,115200);
	sys.usartSetup(serial);
	serial.printf("hoge");
	delay(100);

	pwm.pwmSetup(TIM2_PWM2_Setup,PWM_CNT);
	pwm.duty(0);

	vin.setup(ADC10_Setup);
}



#endif
