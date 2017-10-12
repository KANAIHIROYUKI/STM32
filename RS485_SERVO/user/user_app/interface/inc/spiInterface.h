#ifndef _SPI_INTERCAFE_H_
#define _SPI_INTERCAFE_H_

#include "system.h"
#include "gpio.h"
#include "spi.h"

class SPI_Interface{
public:
	void setup(SPI_Master &spi);

	void nssAssign(GPIO &nss);

	uint16_t begin();
	void end();
	uint16_t available();

	SPI_Master *spi;
	GPIO *nss;
	uint16_t interfaceNumber,takeRequested;
	uint16_t nssSetuped;
};




#endif
