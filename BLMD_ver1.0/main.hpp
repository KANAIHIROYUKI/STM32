#ifndef MAIN_H_
#define MAIN_H_

#include "system.h"
#include "app.h"
#include "base.h"

System sys;

TIM pwm[3];
TIM enc,timer;
GPIO en[3],zeroCross[3],led[2],sw[2],sell[4],hall[3];
ADC cs;

CAN can1;
USART serial;

Switch input[3];

Phase phase[3];
BLDC bldc;

#define DEBUG

int16_t hallToDegree[] = {-1,0,240,300,120,60,180,-1};
uint16_t rxFlag = 0;

#define PRINT_TIME 50
uint64_t printTime = 0,interruptTime;

uint16_t extiPinNumber = 0;



void setup(){
	sys.setup();

	serial.setup(USART1,921600,PA9,PA10);
	sys.usartSetup(serial);
	serial.printf("\n\rFILE = %s\n\rDATE = %s\n\rTIME = %s\n\r",__FILE__,__DATE__,__TIME__);

	timer.timerSetup(TIM1);

	enc.encoderSetup(TIM2,PA0,PA1);

	pwm[1].pwmSetup(TIM3,1,PA6,3600);
	pwm[0].pwmSetup(TIM3,2,PA7,3600);
	pwm[2].pwmSetup(TIM3,3,PA8,3600);

	en[1].setup(PA3,OUTPUT);
	en[0].setup(PA4,OUTPUT);
	en[2].setup(PA5,OUTPUT);

	phase[0].setup(pwm[0],en[0]);
	phase[1].setup(pwm[1],en[1]);
	phase[2].setup(pwm[2],en[2]);

	cs.setup(ADC1,2,PA2);

	bldc.setup(phase[0],phase[1],phase[2]);

	led[0].setup(PB5,OUTPUT);
	led[1].setup(PB6,OUTPUT);

	sw[0].setup(PB7,INPUT_PU);
	sw[1].setup(PB8,INPUT_PU);

	hall[0].setup(PA15,INPUT_PU);
	hall[1].setup(PB3,INPUT_PU);
	hall[2].setup(PB4,INPUT_PU);

	input[0].setup(hall[0]);
	input[1].setup(hall[1]);
	input[2].setup(hall[2]);

	zeroCross[0].setup(PB12,INPUT_PU);
	zeroCross[1].setup(PB13,INPUT_PU);
	zeroCross[2].setup(PB14,INPUT_PU);

	zeroCross[0].interruptSetup(EXTI_Trigger_Rising_Falling);
	zeroCross[1].interruptSetup(EXTI_Trigger_Rising_Falling);
	zeroCross[2].interruptSetup(EXTI_Trigger_Rising_Falling);

	sell[0].setup(PC15,INPUT_PU);
	sell[1].setup(PC14,INPUT_PU);
	sell[2].setup(PB9,INPUT_PU);
	sell[3].setup(PC13,INPUT_PU);

	can1.setup(CAN1,PA12,PA11);

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
}

extern "C" void USB_LP_CAN1_RX0_IRQHandler(void){
	rxFlag++;
	can1.receive();
	return;
}

#endif
