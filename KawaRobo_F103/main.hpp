#ifndef MAIN_H_
#define MAIN_H_

#include "system.h"
#include "app.h"
#include "KawaRoboMain.h"
#include "util.h"

#define IntervalTime 100

System sys;

USART serial;
SPI_Master spi;
GPIO nss,nssMouse,led,sw;

SerialArduino sa;
SBUS sbus;

ADNS9800 mouse;

uint64_t intervalTime;

void setup(){
	sys.setup();

	serial.setup(USART1,115200,PA9,PA10);
	serial.printf("DATE = %s\n\r",__DATE__);
	serial.printf("TIME = %s\n\r",__TIME__);

	sys.usartSetup(serial);

	sa.setup(USART3,115200,PB10,PB11);
	sbus.setup(USART2,100000,PA2,PA3);

	sw.setup(PB0,INPUT_PU);
	led.setup(PC13,OUTPUT);
	led.write(0);

	spi.setup(SPI2,PB13,PB14,PB15);
	nssMouse.setup(PB12,OUTPUT);
	delay(10);

	intervalTime = 0;

}

#endif
