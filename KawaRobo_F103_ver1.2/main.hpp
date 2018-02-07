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
Switch sw[2];
LED led[6];
ADC analog[4],vBattIn;

TIM pwmP[4],pwmN[4];
GPIO individual,motorEn;
NCP5359 motor[4];

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

	pwmP[0].pwmSetup(TIM3,4,PB1,20000);
	pwmP[1].pwmSetup(TIM3,2,PA7,20000);
	pwmP[2].pwmSetup(TIM4,4,PB9,20000);
	pwmP[3].pwmSetup(TIM4,2,PB7,20000);

	pwmN[0].pwmSetup(TIM3,3,PB0,20000);
	pwmN[1].pwmSetup(TIM3,1,PA6,20000);
	pwmN[2].pwmSetup(TIM4,3,PB8,20000);
	pwmN[3].pwmSetup(TIM4,1,PB6,20000);


	for(int i=0;i<4;i++){
		motor[i].setup(pwmP[i],pwmN[i]);
	}

	analog[0].setup(ADC1,0,PA0);
	analog[1].setup(ADC2,1,PA1);
	analog[2].setup(ADC1,4,PA4);
	analog[3].setup(ADC2,5,PA5);

	/*********↓通信↑モーター********/
	serial.setup(USART1,115200,PA9,PA10);
	serial.printf("DATE = %s\n\r",__DATE__);
	serial.printf("TIME = %s\n\r",__TIME__);

	sys.usartSetup(serial);

	sa.setup(USART3,115200,PB10,PB11);
	sbus.setup(USART2,PA2,PA3);

	sw[0].setup(PB5,INPUT_PU);
	sw[1].setup(PB4,INPUT_PU);

	led[0].setup(PC13);
	led[1].setup(PC14);
	led[2].setup(PC15);
	led[3].setup(PA2);
	led[4].setup(PB3);
	led[5].setup(PA15);

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);

	kw.setup(serial,sbus,sa,motor[0],motor[1],motor[2],motor[3],motorEn);
	kw.uiSetup(sw[1],sw[0],led[0],led[1],led[2],led[3],led[4],led[5]);

	if(individual.read()){
		kw.sensorSetup(analog[0],analog[1],analog[2],analog[3],ADC_TO_BATT_BOARD1);
		serial.printf("nr\rboard = 1\n\r");
	}else{
		kw.sensorSetup(analog[0],analog[1],analog[2],analog[3],ADC_TO_BATT_BOARD2);
		serial.printf("\n\rboard = 2\n\r");
	}

}



#endif
