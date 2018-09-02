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

#define PWM_FRQ 1000

System sys;

USART serial;
Switch sw[2];
LED led[6];
ADC analog[4],vBattIn;

TIM pwmP[4],pwmN[4];
GPIO individual,motorEn,spiSS;
NCP5359 motor[4];

GPIO outP[4],outN[4];

SerialArduino sa;
SBUS sbus;

SPI_Master spi;
AS504x mag;

KawaRobo kw;

uint64_t intervalTime;

void setup(){
	sys.setup();
	individual.setup(PB2,INPUT_PU);
	motorEn.setup(PA8,OUTPUT);

	outP[0].setup(PB1,OUTPUT);
	outP[1].setup(PA7,OUTPUT);
	outP[2].setup(PB9,OUTPUT);
	outP[3].setup(PB7,OUTPUT);
	outN[0].setup(PB0,OUTPUT);
	outN[1].setup(PA6,OUTPUT);
	outN[2].setup(PB8,OUTPUT);
	outN[3].setup(PB6,OUTPUT);

	serial.setup(USART1,115200,PA9,PA10);
	serial.printf("DATE = %s\n\r",__DATE__);
	serial.printf("TIME = %s\n\r",__TIME__);

	sys.usartSetup(serial);

	sw[0].setup(PB5,INPUT_PU);
	sw[1].setup(PB4,INPUT_PU);

	led[0].setup(PC13);
	led[1].setup(PC14);
	led[2].setup(PC15);
	led[3].setup(PA2);
	led[4].setup(PB3);
	led[5].setup(PA15);

	led[0].interval(500);


	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);


	motorEn.write(1);

	while(1){
		motorEn.write(1);
		outP[0].write(1);
		outN[0].write(0);
		for(int i=0;i<1000;i++){

		}

		motorEn.write(0);
		delayMicros(50);

		motorEn.write(1);
		outP[0].write(0);
		outN[0].write(1);
		for(int i=0;i<1000;i++){
		}

		motorEn.write(0);
		delayMicros(50);
	}


	while(1){
		led[0].cycle();
		sw[0].cycle();
		sw[1].cycle();
		if(sw[0].read() == 0){
			for(int i=0;i<4;i++){
				motor[i].duty(0.5);
			}
		}else if(sw[1].read() == 0){
			for(int i=0;i<4;i++){
				motor[i].duty(0.5);
			}
		}else{
			for(int i=0;i<4;i++){
				motor[i].duty(0);
			}
		}
		delay(100);
	}
}



#endif
