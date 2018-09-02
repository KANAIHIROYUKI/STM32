#ifndef MAIN_H_
#define MAIN_H_

#include "system.h"
#include "app.h"
#include "util.h"


System sys;

USART serial;
Switch sw[2];
LED led[6];

TIM pwmP[4],pwmN[4];
GPIO motorEn,spiSS;
NCP5359 motor[4];

CAN can;
CAN_RX rx[4];

uint64_t intervalTime;

void setup(){
	sys.setup();
	serial.setup(USART1_Setup,115200);
	sys.usartSetup(serial);


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
	motorEn.write(1);

	sw[0].setup(PB5,INPUT_PU);
	sw[1].setup(PB4,INPUT_PU);

	led[0].setup(PC13);
	led[1].setup(PC14);
	led[2].setup(PC15);
	led[3].setup(PA2);
	led[4].setup(PB3);
	led[5].setup(PA15);

	can.setup(CAN1_Setup);

	rx[0].setup(can,0x100);
	rx[1].setup(can,0x101);
	rx[2].setup(can,0x102);
	rx[3].setup(can,0x103);

}

extern "C" void USB_LP_CAN1_RX0_IRQHandler(void){
	can.receive();
	for(int i=0;i<4;i++){
		rx[i].interrupt();
	}

	return;
}


#endif
