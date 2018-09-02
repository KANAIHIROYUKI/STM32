#include "ADNS9800.h"

void ADNS9800::setup(SPI_Master &spi,GPIO &nss) {
	this->nss = &nss;
	this->spi = &spi;
	spiif.setup(spi);
	spiif.nssAssign(nss);

	spiif.end();

	if(spiif.begin() == 0){
		System::error = 8;
	}
	spiif.end();

	write(REG_Power_Up_Reset,0x5a);

	delay(50); // wait for it to reboot
			   // read registers 0x02 to 0x06 (and discard the data)

	//end(); // ensure that the serial port is reset
	//begin(); // ensure that the serial port is reset
	//end(); // ensure that the serial port is reset
	//write(REG_Power_Up_Reset, 0x5a); // force reset
	//delay(50); // wait for it to reboot
			   // read registers 0x02 to 0x06 (and discard the data)
	/*
	read(REG_Motion);
	read(REG_Delta_X_L);
	read(REG_Delta_X_H);
	read(REG_Delta_Y_L);
	read(REG_Delta_Y_H);
	byte laser_ctrl0 = read(REG_LASER_CTRL0);
	uint8_t laser_ctrl0 = 0;
	write(REG_LASER_CTRL0, laser_ctrl0 & 0xf0);	//レーザー出力有効
*/
	delay(1);
}

void ADNS9800::setupRead(){
	read(REG_Motion);
	read(REG_Delta_X_L);
	read(REG_Delta_X_H);
	read(REG_Delta_Y_L);
	read(REG_Delta_Y_H);
	uint8_t laser_ctrl0 = read(REG_LASER_CTRL0);
	write(REG_LASER_CTRL0, laser_ctrl0 & 0xf0);	//レーザー出力有効
}

void ADNS9800::cycle() {
	/*begin();

	xyDiffer[0] = (int)read(REG_Delta_X_L) + (int)(read(REG_Delta_X_H) << 8);
	xyDiffer[1] = (int)read(REG_Delta_Y_L) + (int)(read(REG_Delta_Y_H) << 8);

	xyIntegral[0] += xyDiffer[0];
	xyIntegral[1] += xyDiffer[1];

	end();*/


	if(spiif.begin()){
		transferStat = 1;
		transferTime = micros();
	}

	if(spiif.available() && micros() > transferTime){
		switch(transferStat){
		case 1:

			break;

		case 2:

			break;

		case 3:

			break;

		case 4:

			break;
		}
	}
}


void ADNS9800::write(uint8_t add, uint8_t data) {

	/*if(spiif.begin()){
		spi->write(add | 0x80);
		spi->write(data);
		transferTime = micros() + ADNS9800_DELAY_tNCS_SLCK + ADNS9800_DELAY_tSRAD;

	}

	if(spiif.available()){
		if(micros() > transferTime){
			spiif.end();
		}
	}*/

	spiif.begin();

	spi->transfer(add & 0x7f);
	spi->transfer(data);

	delayMicros(ADNS9800_DELAY_tNCS_SLCK);
	spiif.end();
	delayMicros(ADNS9800_DELAY_tSRAD);

	//begin();

	//SPI.transfer(add | 0x80);
	//SPI.transfer(data);

	//delayMicroseconds(20); // tSCLK-NCS for write operation
	//end();
	//delayMicroseconds(100);
}

uint8_t ADNS9800::read(uint8_t add) {

	spiif.begin();

	spi->transfer(add & 0x7f);
	delayMicros(ADNS9800_DELAY_tSRAD);
	uint8_t data = spi->transfer(0);
	delayMicros(ADNS9800_DELAY_tNCS_SLCK);

	spiif.end();

	//begin();

	//SPI.transfer(add & 0x7f);

	//delayMicroseconds(100); // tSRAD
	//byte data = SPI.transfer(0);

	//delayMicroseconds(1); // tSCLK-NCS for read operation is 120ns
	//end();
	//delayMicroseconds(19); //  tSRW/tSRR (=20us) minus tSCLK-NCS

	return data;
}
