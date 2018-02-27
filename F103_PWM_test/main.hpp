#ifndef MAIN_H_
#define MAIN_H_

#include "system.h"
#include "app.h"
#include "KawaRoboMain.h"
#include "util.h"

#define GYRO_OFFSET_BOARD1 -20
#define GYRO_OFFSET_BOARD2 110

#define ADC_TO_BATT_BOARD1 16.901
#define ADC_TO_BATT_BOARD2 17.114

System sys;

USART serial;
TIM pwm;
ADC adc;

uint64_t intervalTime;

void setup(){
	sys.setup();
	serial.setup(USART1,115200,PA9,PA10);
	serial.printf("DATE = %s\n\r",__DATE__);
	serial.printf("TIME = %s\n\r",__TIME__);

	sys.usartSetup(serial);
	pwm.pwmSetup(TIM1,1,PA8,18000,8);
	pwm.duty(0);
	adc.setup(ADC1,0,PA0);
}



#endif
