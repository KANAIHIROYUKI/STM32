#ifndef MAIN_H_
#define MAIN_H_

#include "system.h"
#include "app.h"
#include "base.h"

#define IntervalTime 100

System sys;

USART serial;
ADC a0,a1;

uint8_t debugMode = 0,printValue = 0,canData[8];
uint64_t intervalTimer = 0;


void setup(){
	sys.setup();

	serial.setup(USART3,115200,PB10,PB11);
	serial.printf("\n\rFILE = %s\n\r",__FILE__);
	serial.printf("DATE = %s\n\r",__DATE__);
	serial.printf("TIME = %s\n\r",__TIME__);

	a0.setup(ADC1,0,PA0);
	a1.setup(ADC1,1,PA1);

	delay(100);
}



extern "C" void USB_LP_CAN1_RX0_IRQHandler(void){

	return;
}

#endif
