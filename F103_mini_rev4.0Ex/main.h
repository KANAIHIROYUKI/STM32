#include "stm32f10x.h"
#include "stdio.h"
#include "system.h"
#include "app.h"



#define IntervalTime 50

uint16_t rxFlag;

ADC v1a,v1b,v2a,v2b,vbatt;
USART serial,vserial;

void setup(){
	systemSetup();

	vserial.setup(USART2,115200,PA2,PA3);


	v1a.setup(ADC1,0,PA0);
	v1b.setup(ADC1,1,PA1);

	v2a.setup(ADC1,3,PA3);
	v2b.setup(ADC1,8,PB0);
	vbatt.setup(ADC2,9,PB1);


	serial.setup(USART1,921600,PA9,PA10);
	serial.printf("\n\rFILE = %s\n\r",__FILE__);
	serial.printf("DATE = %s\n\r",__DATE__);
	serial.printf("TIME = %s\n\r",__TIME__);

}




extern "C" void USB_LP_CAN1_RX0_IRQHandler(void){
	rxFlag++;
	return;
}

