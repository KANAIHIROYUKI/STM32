#ifndef MAIN_H_
#define MAIN_H_

#include "system.h"
#include "app.h"
#include "KawaRoboMain.h"
#include "util.h"
#include "PID_Controller.h"

#define IntervalTime 100

System sys;

USART serial;
SPI_Master spi;
GPIO nss,nssMouse;
Switch sw[2];
LED led[4];

TIM pwm[4];
GPIO cw[4];
NCP5359 motor[4];

SerialArduino sa;
SBUS sbus;
CAN can;

ADNS9800 mouse;

KawaRobo kw;

uint64_t intervalTime;

void setup(){
	sys.setup();
/*
	pwm[0].pwmSetup(TIM4,4,PB9,2000);	//かわロボ基板用
	pwm[1].pwmSetup(TIM4,3,PB8,2000);
	pwm[2].pwmSetup(TIM4,2,PB7,2000);
	pwm[3].pwmSetup(TIM4,1,PB6,2000);

	cw[0].setup(PB5,OUTPUT);
	cw[1].setup(PB4,OUTPUT);
	cw[2].setup(PB3,OUTPUT);
	cw[3].setup(PA15,OUTPUT);
	//*/

	pwm[0].pwmSetup(TIM3,2, PB5,20000);
	pwm[1].pwmSetup(TIM3,1, PB4,20000);
	pwm[2].pwmSetup(TIM2,2, PB3,20000);
	pwm[3].pwmSetup(TIM2,1,PA15,20000);

	cw[0].setup(PB9,OUTPUT);
	cw[1].setup(PB8,OUTPUT);
	cw[2].setup(PB7,OUTPUT);
	cw[3].setup(PB6,OUTPUT);


	motor[0].setup(pwm[0],cw[0]);
	motor[1].setup(pwm[1],cw[1]);
	motor[2].setup(pwm[2],cw[2]);
	motor[3].setup(pwm[3],cw[3]);
	/*
	for(int i=0;i<4;i++){
		motor[i].setup(pwm[i],cw[i]);
	}*/

	/*********↓通信↑モーター********/

	serial.setup(USART1,115200,PA9,PA10);
	serial.printf("DATE = %s\n\r",__DATE__);
	serial.printf("TIME = %s\n\r",__TIME__);

	sys.usartSetup(serial);

	sa.setup(USART3,115200,PB10,PB11);
	sbus.setup(USART2,100000,PA2,PA3);

	can.setup(CAN1,PA12,PA11);

	sw[0].setup(PB0,INPUT_PU);
	sw[1].setup(PA7,INPUT_PU);

	led[0].setup(PC13);
	led[1].setup(PC14);
	led[2].setup(PC15);
	led[3].setup(PB2);

	/*
	spi.setup(SPI2,PB13,PB14,PB15);
	nssMouse.setup(PB12,OUTPUT);
	delay(10);
	*/

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2,ENABLE);
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3,ENABLE);


	kw.setup(serial,sbus,sa,motor[0],motor[1],motor[2],motor[3]);
	kw.uiSetup(sw[0],sw[1],led[0],led[1],led[2],led[3]);

	intervalTime = 0;

}

#endif
