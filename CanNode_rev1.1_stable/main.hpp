#ifndef MAIN_H_
#define MAIN_H_

#include "system.h"
#include "app.h"

#define CAN_SELF_DEBUG

TIM enc[4];
GPIO io[8],led;
USART serial;

CAN can1;

CanNodeValve canVlv;
CanNodeEncoder canEnc[4];

CanEncoder canEncoder[4];
CanValve canValve;

//#define DEBUG

#define PRINT_TIME 100


uint16_t rxFlag = 0;
uint64_t printTime = 0;

void setup(){
	systemSetup();

	serial.setup(USART3,921600,PB10,PB11);
	serial.printf("\n\rFILE = %s\n\rDATE = %s\n\rTIME = %s\n\r",__FILE__,__DATE__,__TIME__);

	enc[0].encoderSetup(TIM1,PA8,PA9);
	enc[1].encoderSetup(TIM2,PA0,PA1);
	enc[2].encoderSetup(TIM3,PA6,PA7);
	enc[3].encoderSetup(TIM4,PB6,PB7);

	enc[0].reverse();

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

	canVlv.setup(can1,0);
	canVlv.pinAdd(io[0]);
	canVlv.pinAdd(io[1]);
	canVlv.pinAdd(io[2]);
	canVlv.pinAdd(io[3]);
	canVlv.pinAdd(io[4]);
	canVlv.pinAdd(io[5]);
	canVlv.pinAdd(io[6]);
	canVlv.pinAdd(io[7]);

	led.write(Bit_RESET);

#ifdef CAN_SELF_DEBUG
	canEncoder[0].setup(can1,0,10);
	canEncoder[1].setup(can1,1,10);
	canEncoder[2].setup(can1,2,10);
	canEncoder[3].setup(can1,3,10);

	canValve.setup(can1,0);
#endif


}

extern "C" void USB_LP_CAN1_RX0_IRQHandler(void){
	rxFlag++;
	can1.receive();
	canVlv.interrupt();
	canEnc[0].interrupt();
	canEnc[1].interrupt();
	canEnc[2].interrupt();
	canEnc[3].interrupt();

#ifdef CAN_SELF_DEBUG
	canEncoder[0].interrupt();
	canEncoder[1].interrupt();
	canEncoder[2].interrupt();
	canEncoder[3].interrupt();
#endif
	return;
}

#endif
