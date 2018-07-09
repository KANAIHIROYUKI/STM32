#ifndef DRV8305_H_
#define DRV8305_H_

#include "system.h"
#include "spi.h"



class DRV8305{
	void setup(SPI_Master &spi,GPIO &ss);

	SPI_Master *spi;
	GPIO *ss;
};






#endif
