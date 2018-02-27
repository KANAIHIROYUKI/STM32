#ifndef _AS504x_H_
#define _AS504x_H_

#include "spi.h"
#include "gpio.h"
#include "enc.h"
#include "spiInterface.h"

#define AS5040_SPI_TIMEOUT 10


class AS504x{
public:
	void setup(SPI_Master &spi,GPIO &ss);
	void addEnc(Enc &enc);

	void cycle();

	uint64_t read(uint16_t deviceNumber);
	void reset();
	void reverse();


	uint64_t countValue[8],revolutionCnt[8],spiStartTime;
	uint16_t directlyValue[8],oldDirectryValue[8];

	uint16_t magencDeviceNumber,revCnt,spiInterfaceNumber,valueReadCnt;
//private:
	Enc *enc[8];
	SPI_Master *spi;
	SPI_Interface spiif;
	GPIO *nss;
};



#endif
