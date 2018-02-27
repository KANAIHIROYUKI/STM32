#ifndef MAIN_H_
#define MAIN_H_

#include "system.h"
#include "app.h"
#include "util.h"

#define GYRO_OFFSET_BOARD1 -20
#define GYRO_OFFSET_BOARD2 110

#define ADC_TO_BATT_BOARD1 16.901
#define ADC_TO_BATT_BOARD2 17.114

System sys;

USART serial;
CAN can;
Switch sw[2];
LED led[6];
ADC analog[4],vBattIn;

TIM pwmP[4],pwmN[4];
GPIO individual,motorEn,spiNss;
NCP5359 motor[4];

SerialArduino sa;
SerialArduino ps3;

CAN_RX rx[4];
CAN_TX tx[4];
Mecanum mc;

CanMotorDriver md[4];

uint64_t intervalTime,intTime;
uint8_t data[8],swStat[8];
int servoDeg[8];

void setup(){
	sys.setup();
	individual.setup(PB2,INPUT_PU);
	motorEn.setup(PA8,OUTPUT);

	pwmP[0].pwmSetup(TIM3,4,PB1,2000);
	pwmP[1].pwmSetup(TIM3,2,PA7,2000);
	pwmP[2].pwmSetup(TIM4,4,PB9,2000);
	pwmP[3].pwmSetup(TIM4,2,PB7,2000);

	pwmN[0].pwmSetup(TIM3,3,PB0,2000);
	pwmN[1].pwmSetup(TIM3,1,PA6,2000);
	pwmN[2].pwmSetup(TIM4,3,PB8,2000);
	pwmN[3].pwmSetup(TIM4,1,PB6,2000);


	for(int i=0;i<4;i++){
		motor[i].setup(pwmP[i],pwmN[i]);
	}

	analog[0].setup(ADC1,0,PA0);
	analog[1].setup(ADC2,1,PA1);
	analog[2].setup(ADC1,4,PA4);
	analog[3].setup(ADC2,5,PA5);

	/*********↓通信↑モーター********/

	ps3.setup(USART1,115200,PA9,PA10);
	serial.setup(USART1,115200,PA9,PA10);
	serial.printf("DATE = %s\n\r",__DATE__);
	serial.printf("TIME = %s\n\r",__TIME__);

	sys.usartSetup(serial);

	sa.setup(USART3,115200,PB10,PB11);

	sw[0].setup(PB5,INPUT_PU);
	sw[1].setup(PB4,INPUT_PU);

	led[0].setup(PC13);
	led[1].setup(PC14);
	led[2].setup(PC15);
	led[3].setup(PA2);
	led[4].setup(PB3);
	led[5].setup(PA15);

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);

	can.setup(CAN1,PA12,PA11);

	if(individual.read()){	//master
		for(int i=0;i<4;i++){
			//tx[i].setup(can,0x100 + i);
			//rx[i].setup(can,0x100 + i);
		}
	}else{					//node
		for(int i=0;i<4;i++){
			//tx[i].setup(can,0x100 + i);
			rx[i].setup(can,0x100 + i);
		}
	}

	for(int i=0;i<4;i++){
		md[i].setup(can,i);
	}
}

extern "C" void USB_LP_CAN1_RX0_IRQHandler(void){
	can.receive();
	for(int i=0;i<4;i++){
		rx[i].interrupt();
	}

	return;
}

#endif
