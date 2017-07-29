#ifndef MAIN_H_
#define MAIN_H_

#include "system.h"
#include "app.h"
#include "base.h"

System sys;

TIM enc[4];

CAN can1;
USART serial;
GPIO io[8],led;

CanNodeEncoder canEnc[4];

#define PRINT_TIME 50

uint64_t printTime = 0,canLastReceiveTime = 0;

void setup(){
	sys.setup();

	serial.setup(USART3,115200,PB10,PB11);
	sys.usartSetup(serial);
	serial.printf("\n\rFILE = %s\n\rDATE = %s\n\rTIME = %s\n\r",__FILE__,__DATE__,__TIME__);

	led.setup(PB2,OUTPUT);

	enc[0].encoderSetup(TIM4,PB6,PB7);
	enc[1].encoderSetup(TIM1,PA8,PA9);
	enc[2].encoderSetup(TIM2,PA0,PA1);
	enc[3].encoderSetup(TIM3,PA6,PA7);

	enc[1].reverse();

	can1.setup(CAN1,PA12,PA11);

	canEnc[0].setup(enc[0],can1,0);
	canEnc[1].setup(enc[1],can1,1);
	canEnc[2].setup(enc[2],can1,2);
	canEnc[3].setup(enc[3],can1,3);

	enc[0].reset();
	enc[1].reset();
	enc[2].reset();
	enc[3].reset();

	delay(10);
	serial.printf("can rx filter\n\r");
	for(int i=0;i<can1.filterCnt;i++){
		serial.printf("add = 0x%x\n\r",can1.filterAddress[i]);
	}
	delay(10);
}

extern "C" void USB_LP_CAN1_RX0_IRQHandler(void){
	can1.receive();

	canEnc[0].interrupt();
	canEnc[1].interrupt();
	canEnc[2].interrupt();
	canEnc[3].interrupt();

	canLastReceiveTime = millis();
	return;
}

#endif
