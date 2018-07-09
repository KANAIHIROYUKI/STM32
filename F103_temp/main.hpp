#ifndef MAIN_H_
#define MAIN_H_

#include "system.h"
#include "app.h"
#include "util.h"


System sys;

USART serial;
GPIO ss;
SPI_Master spi;
AS504x enc;
SBDBT ps3;
LED led[3];
uint64_t intervalTime;
/*
uint32_t FlashRead(uint32_t address);
void FlashWrite(uint32_t address,uint32_t data);
bool FlashErase();*/

void setup(){
	sys.setup();
	serial.setup(USART1_Setup,115200);
	sys.usartSetup(serial);

	spi.setup(SPI1,PA5,PA6,PA7);
	ss.setup(PA4,OUTPUT);
	enc.setup(spi,ss);


	ps3.setup(USART2_Setup,115200);

	led[0].setup(PC14);
	led[0].gpio.IOInvert();
	led[1].setup(PC15);
	led[2].setup(PC13);
	led[0].interval(500);
	led[1].interval(500);
	led[2].interval(250);


}



#endif
