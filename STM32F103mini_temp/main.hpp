#ifndef MAIN_H_
#define MAIN_H_

#include "system.h"
#include "app.h"
#include "KawaRoboMain.h"
#include "util.h"

#define IntervalTime 100

System sys;

USART serial,sbusIn,saIn;
Switch sw[2];
GPIO led[4],rs485dir,cw[4],spiSS;
TIM pwm[4];
SPI_Master spi;

NCP5359 motor[4];
AS504x magEnc;
Enc enc[4];
SBUS sbus;
SerialArduino sa;

KawaRobo kw;
MotorController mc[4];

void setup(){
	sys.setup();

	/*
	pwm[0].pwmSetup(TIM4,3,PB9,2000);
	pwm[1].pwmSetup(TIM4,2,PB8,2000);
	pwm[2].pwmSetup(TIM4,1,PB7,2000);
	pwm[3].pwmSetup(TIM4,0,PB6,2000);

	cw[0].setup(PB5,OUTPUT);
	cw[1].setup(PB4,OUTPUT);
	cw[2].setup(PB3,OUTPUT);
	cw[3].setup(PA15,OUTPUT);

	spi.setup(SPI2,PB13,PB14,PB15);
	spiSS.setup(PB12,OUTPUT);
	spiSS.write(1);
	//magEnc.setup(spi,spiSS);

	for(int i=0;i<4;i++){
		motor[i].setup(pwm[i],cw[i]);
		//magEnc.addEnc(enc[i]);
		mc[i].setup(motor[i],enc[i],1024);
	}

	led[0].setup(PC13,OUTPUT);
	led[1].setup(PC14,OUTPUT);
	led[2].setup(PC15,OUTPUT);
	led[3].setup(PB2,OUTPUT);

	sw[0].setup(PB0,INPUT_PU);
	sw[1].setup(PA7,INPUT_PU);

	serial.setup(USART1,115200,PA9,PA10);
	//serial.printf("\n\rFILE = %s\n\r",__FILE__);
	serial.printf("DATE = %s\n\r",__DATE__);
	serial.printf("TIME = %s\n\r",__TIME__);

	sbusIn.setup(USART2,100000,PA2,PA3);
	sbus.setup(sbusIn);

	saIn.setup(USART3,115200,PB10,PB11);

	kw.setup(serial,sbus,sa,motor[0],motor[1],motor[2],motor[3]);
	kw.uiSetup(sw[0],sw[1],led[0],led[1],led[2],led[3]);

	for(int i=0;i<100;i++){
		spi.write(i);
	}
	delay(10);

	//spiSS.write(1);
	for(int i=0;i<100;i++){
		serial.printf("data = %d,",spi.read());
		delay(1);
	}

	delay(10);
	serial.printf("interrupt = %d\n\r",spi.interruptCnt);
	delay(10);*/
}

#endif
