#ifndef MAIN_H_
#define MAIN_H_

#include "system.h"
#include "app.h"
#include "base.h"

System sys;

TIM pwm[3];
TIM enc;
GPIO en[3],zeroCross[3],led;
ADC cs[3];

CAN can1;
USART serial;

Phase phase[3];
BLDC bldc;

#define DEBUG
uint16_t rxFlag = 0;

#define PRINT_TIME 50
uint64_t printTime = 0;


void setup(){
	sys.setup();

	serial.setup(USART3,921600,PB10,PB11);
	sys.usartSetup(serial);
	//serial.printf("\n\rFILE = %s\n\rDATE = %s\n\rTIME = %s\n\r",__FILE__,__DATE__,__TIME__);

	enc.encoderSetup(TIM4,PB6,PB7);
	pwm[0].pwmSetup(TIM2,1,PA0,3600);
	pwm[1].pwmSetup(TIM2,2,PA1,3600);
	pwm[2].pwmSetup(TIM2,3,PA2,3600);

	phase[0].setup(pwm[0],en[0]);
	phase[1].setup(pwm[1],en[1]);
	phase[2].setup(pwm[2],en[2]);

	cs[0].setup(ADC1,5,PA4);
	cs[1].setup(ADC1,6,PA5);
	cs[2].setup(ADC1,7,PA6);

	phase[0].adcAssigne(cs[0]);
	phase[1].adcAssigne(cs[1]);
	phase[2].adcAssigne(cs[2]);

	bldc.setup(phase[0],phase[1],phase[2]);

	led.setup(PC13,OUTPUT);

	can1.setup(CAN1,PA12,PA11);
	//led.write(1);
}

extern "C" void USB_LP_CAN1_RX0_IRQHandler(void){
	rxFlag++;
	can1.receive();
	return;
}

#endif
