#ifndef MAIN_H_
#define MAIN_H_

#include "system.h"
#include "app.h"

TIM enc[4];
GPIO io[8],led;
USART serial;

CAN can1;

CanNodeValve canValve;
CanNodeEncoder canEnc[4];

#define DEBUG

#define PRINT_TIME 100


uint16_t rxFlag = 0;
uint64_t printTime = 0;

void setup(){
	systemSetup();

	serial.setup(USART3,921600,PB10,PB11);

	enc[0].encoderSetup(TIM1,PA8,PA9);
	enc[1].encoderSetup(TIM2,PA0,PA1);
	enc[2].encoderSetup(TIM3,PA6,PA7);
	enc[3].encoderSetup(TIM4,PB6,PB7);

	led.setup(PB2,OUTPUT);

	io[0].setup(PA2,OUTPUT);
	io[1].setup(PA3,OUTPUT);
	io[2].setup(PB0,OUTPUT);
	io[3].setup(PB1,OUTPUT);
	io[4].setup(PA4,OUTPUT);
	io[5].setup(PA5,OUTPUT);
	io[6].setup(PB12,OUTPUT);
	io[7].setup(PB13,OUTPUT);

	can1.setup(CAN1,PA12,PA11);

	//******************Å´CAN node setup

	canEnc[0].setup(enc[0],can1,0);
	canEnc[1].setup(enc[1],can1,1);
	canEnc[2].setup(enc[2],can1,2);
	canEnc[3].setup(enc[3],can1,3);

	canValve.setup(can1,0);
	canValve.pinAdd(io[0]);
	canValve.pinAdd(io[1]);
	canValve.pinAdd(io[2]);
	canValve.pinAdd(io[3]);
	canValve.pinAdd(io[4]);
	canValve.pinAdd(io[5]);
	canValve.pinAdd(io[6]);
	canValve.pinAdd(io[7]);

	led.write(Bit_RESET);


	serial.printf("\n\rFILE = %s\n\rDATE = %s\n\rTIME = %s\n\r",__FILE__,__DATE__,__TIME__);
}

extern "C" void USB_LP_CAN1_RX0_IRQHandler(void){
	rxFlag++;
	can1.receive();
	canValve.interrupt();
	canEnc[0].interrupt();
	canEnc[1].interrupt();
	canEnc[2].interrupt();
	canEnc[3].interrupt();
	return;
}

#endif
