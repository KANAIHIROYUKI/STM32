#include "stm32f10x.h"
#include "stdio.h"
#include "system.h"
#include "app.h"



#define IntervalTime 100

uint64_t intervalTime = 0;
uint16_t rxFlag;


GPIO signal,led[8],toggleSw[8],rotarySw[8];
USART serial;
CAN can1;

CanNodeValve canVlv;

CanValve canValve;

void setup(){


	systemSetup();

	//signal.setup(PB0,OUTPUT);

	led[0].setup(PC15,OUTPUT);
	led[1].setup(PC14,OUTPUT);
	led[2].setup(PC15,OUTPUT);
	led[3].setup(PA0,OUTPUT);
	led[4].setup(PA1,OUTPUT);
	led[5].setup(PA2,OUTPUT);
	led[6].setup(PA3,OUTPUT);
	led[7].setup(PA4,OUTPUT);

	rotarySw[0].setup(PA5,INPUT_PU);
	rotarySw[1].setup(PA6,INPUT_PU);
	rotarySw[2].setup(PA7,INPUT_PU);
	rotarySw[3].setup(PB0,INPUT_PU);
	rotarySw[4].setup(PB1,INPUT_PU);
	rotarySw[5].setup(PB2,INPUT_PU);
	rotarySw[6].setup(PB10,INPUT_PU);
	rotarySw[7].setup(PB11,INPUT_PU);

	toggleSw[0].setup(PB9,INPUT_PU);
	toggleSw[1].setup(PB8,INPUT_PU);
	toggleSw[2].setup(PB7,INPUT_PU);
	toggleSw[3].setup(PB6,INPUT_PU);
	toggleSw[4].setup(PB5,INPUT_PU);
	toggleSw[5].setup(PB4,INPUT_PU);
	toggleSw[6].setup(PB3,INPUT_PU);
	toggleSw[7].setup(PA15,INPUT_PU);

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);

	serial.setup(USART1,921600,PA9,PA10);
	can1.setup(CAN1,PA12,PA11);

	canVlv.setup(can1,1);
	canVlv.pinAdd(led[0]);
	canVlv.pinAdd(led[1]);
	canVlv.pinAdd(led[2]);
	canVlv.pinAdd(led[3]);
	canVlv.pinAdd(led[4]);
	canVlv.pinAdd(led[5]);
	canVlv.pinAdd(led[6]);
	canVlv.pinAdd(led[7]);

	canValve.setup(can1,1);

	serial.printf("\n\rFILE = %s\n\r",__FILE__);
	serial.printf("DATE = %s\n\r",__DATE__);
	serial.printf("TIME = %s\n\r",__TIME__);

}




extern "C" void USB_LP_CAN1_RX0_IRQHandler(void){
	rxFlag++;
	can1.receive();
	canVlv.interrupt();
	return;
}

