#ifndef MAIN_H_
#define MAIN_H_

#include "system.h"
#include "app.h"
#include "util.h"

#define FLASH_PAGE_ADDRESS 0x801F800

System sys;

USART serial;
GPIO ss;
SPI_Master spi;

uint64_t intervalTime;
uint32_t FlashRead(uint32_t address);
void FlashWrite(uint32_t address,uint32_t data);
int FlashErase();

void setup(){
	sys.setup();
	serial.setup(USART1,115200,PA9,PA10);
	serial.printf("DATE = %s\n\r",__DATE__);
	serial.printf("TIME = %s\n\r",__TIME__);

	spi.setup(SPI1,PA5,PA6,PA7);
	ss.setup(PA4,OUTPUT);

	sys.usartSetup(serial);

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
}



#endif
