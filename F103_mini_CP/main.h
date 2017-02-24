#include "stm32f10x.h"
#include "stdio.h"
#include "system.h"
#include "app.h"



#define IntervalTime 100

#define CAN_ADD_SWITCH 0x700

#define ROTARY 15 - ((switchPin[3].read() << 3) + (switchPin[0].read() << 2) + (switchPin[2].read() << 1) + (switchPin[1].read() << 0))

uint64_t intervalTime = 0;
uint16_t rxFlag;


GPIO signal,led[16],switchPin[8];
USART serial;
CAN can1;

CanNodeValve canVlv[2];

CanValve canValve[2];

void setup(){


	systemSetup();



	led[0].setup(PB9,OUTPUT);
	led[1].setup(PB8,OUTPUT);
	led[2].setup(PB7,OUTPUT);
	led[3].setup(PB6,OUTPUT);
	led[4].setup(PB5,OUTPUT);
	led[5].setup(PA1,OUTPUT);
	led[6].setup(PA2,OUTPUT);
	led[7].setup(PA3,OUTPUT);

	led[8].setup(PA8,OUTPUT);
	led[9].setup(PB15,OUTPUT);
	led[10].setup(PB14,OUTPUT);
	led[11].setup(PB13,OUTPUT);
	led[12].setup(PB12,OUTPUT);
	led[13].setup(PB2,OUTPUT);
	led[14].setup(PB10,OUTPUT);
	led[15].setup(PB11,OUTPUT);

	for(int i=0;i<16;i++){
		led[i].write(0);
	}

	switchPin[0].setup(PC13,INPUT_PU);
	switchPin[1].setup(PC14,INPUT_PU);
	switchPin[2].setup(PC15,INPUT_PU);
	switchPin[3].setup(PA0,INPUT_PU);

	switchPin[4].setup(PA5,INPUT_PU);
	switchPin[5].setup(PA6,INPUT_PU);
	switchPin[6].setup(PA7,INPUT_PU);
	switchPin[7].setup(PB1,INPUT_PU);


	can1.setup(CAN1,PA12,PA11);
	can1.filterAdd(CAN_ADD_SWITCH);

	canVlv[0].setup(can1,1);
	canVlv[1].setup(can1,2);

	for(int i=0;i<8;i++){
		canVlv[0].pinAdd(led[i]);
		canVlv[1].pinAdd(led[i + 8]);
	}

	canValve[0].setup(can1,1);
	canValve[1].setup(can1,2);

	serial.setup(USART1,921600,PA9,PA10);
	serial.printf("\n\rFILE = %s\n\r",__FILE__);
	serial.printf("DATE = %s\n\r",__DATE__);
	serial.printf("TIME = %s\n\r",__TIME__);
}




extern "C" void USB_LP_CAN1_RX0_IRQHandler(void){
	rxFlag++;
	can1.receive();
	canVlv[0].interrupt();
	canVlv[1].interrupt();
	return;
}
