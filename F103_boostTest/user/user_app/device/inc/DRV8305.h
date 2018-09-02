#ifndef DRV8305_H_
#define DRV8305_H_

#include "system.h"
#include "spi.h"

//オーバーフローを用いるため8つまである
//uint8_t BLDC_switchState[][] = {{1,1,1,0,0,0,1,1},{0,0,1,1,1,0,0,0},{1,0,0,0,1,1,1,0}};

class DRV8305{
public:
	void setup(SPI_Master &spi,GPIO &ss);
	void programing();
	void write(uint8_t address,uint16_t data);
	void readFault();
	void resetFault();
	void setAmpDCCariblation();
	void setAmpNormalOperation();

	uint16_t read(uint8_t address);

	uint16_t data[16];
	SPI_Master *spi;
	GPIO *ss;
	//SPIモードは1？

	void setupGPIO();
	uint16_t isGdOk();

	void setupAmpRegister(uint16_t registerValue);
	float adcToCurrentValue;

	void cariblation();
	int16_t adcOffset[3];

	float readCurrent(uint8_t phase);

	void cycleAdc();
	uint16_t adcValue[3];

	ADC cs[3];
	TIM pwm[6];
	GPIO nFault,pwGood,gdWake,state[3],dwell;
	//nFaultとpwGoodはODなのでプルアップすること

};






#endif
