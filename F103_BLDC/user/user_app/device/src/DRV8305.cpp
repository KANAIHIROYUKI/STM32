#include "DRV8305.h"

void DRV8305::setup(SPI_Master &_spi,GPIO &_ss){
	this->spi = &_spi;
	this->ss = &_ss;
	data[0x1] = 0x0000;		//Warning & wdg
	data[0x2] = 0x0000;		//OV/VDS Fault
	data[0x3] = 0x0000;		//IC Fault
	data[0x4] = 0x0000;		//VGS Fault

	data[0x5] = (0x3 << 8) + (0x4 << 4) + 0x4;								//HS GD Control
	data[0x6] = (0x3 << 8) + (0x4 << 4) + 0x4;								//LS GD Control
	data[0x7] = (0x1 << 9) + (0x0 << 7) + (0x1 << 4) + (0x1 << 2) + 0x2;	//GD Control
	data[0x8] = 0;															//RSVD
	data[0x9] = (0 << 10) + (0 << 9) + (0 << 8) + (0 << 7) + (0x1 << 5) + (0 << 4) + (0 << 3) + (0 << 2) + (0 << 1) + 0;	//IC Operation
	data[0xA] = (0 << 10) + (0 << 9) + (0 << 8) + (0x0 << 6) + (0x0 << 4) + (0x0 << 2) + 0x0;								//VoltageReg
	data[0xB] = (0x1 << 8) + (0x1 << 3) + (0x0 << 2) + 0x2;																	//VdsCntrol
	data[0xC] = (0x19 << 3) + 0x0;
}

void DRV8305::programing(){
	spi->changeMode(SPI_Mode1);
	write(0x5,(0b11 << 8) + (0b1011 << 4) + 0b1011);
	write(0x6,(0b11 << 8) + (0b1011 << 4) + 0b1011);													//GD Current 1.25A/1.00A
	write(0x7,(0b0 << 9) + (0b10 << 7) + (0b011 << 4) + (0b1 << 2) + 0b10);							//DT = 440ns
	write(0xA,(0 << 10) + (0 << 9) + (0 << 8) + (0b0 << 6) + (0b00 << 4) + (0b00 << 2) + 0b00);		//10V/V
	write(0xC,(0b10010 << 3) + 0b0);																//Vds0.511V
}

void DRV8305::readFault(){
	data[1] = read(0x1);
	data[2] = read(0x2);
	data[3] = read(0x3);
	data[4] = read(0x4);
}

void DRV8305::resetFault(){
	spi->changeMode(SPI_Mode1);
	write(0x1,0);
	write(0x2,0);
	write(0x3,0);
	write(0x4,0);
}

void DRV8305::setAmpDCCariblation(){
	spi->changeMode(SPI_Mode1);
	write(0xA,data[0xA] & (0b111 << 8));
}

void DRV8305::setAmpNormalOperation(){
	spi->changeMode(SPI_Mode1);
	write(0xA,data[0xA] | 0xFF);

}

void DRV8305::write(uint8_t address,uint16_t _data){
	if(address == 0x8 || address > 0xC)return;
	spi->changeMode(SPI_Mode1);
	ss->write(0);
	delayMicros(100);
	spi->transfer(0x00 + (address << 3) + (_data >> 8));
	spi->transfer(_data);
	ss->write(1);
	data[address] = _data;
}

uint16_t DRV8305::read(uint8_t address){
	if(address == 0x8 || address > 0xC)return 0;
	spi->changeMode(SPI_Mode1);
	ss->write(0);
	delayMicros(100);
	rxData = spi->transfer(0x80 + (address << 3)) << 8;
	rxData += spi->transfer(0x00);
	ss->write(1);
	delayMicros(200);
	return rxData;
}

void DRV8305::setupGPIO(){
	gdWake.write(1);
	ss->write(1);
}

uint16_t DRV8305::isGdOk(){
	if(pwGood.read() == 1){
		if(nFault.read() == 0){
			return 1;
		}else{
			return 0;
		}
	}else{
		return 0;
	}
}

void DRV8305::setupAmpRegister(uint16_t registerValue){
	adcToCurrentValue = registerValue * 4095;
	switch(data[0xA] & 0b11){
	case 0b00:
		adcToCurrentValue *= 10;
		break;
	case 0b01:
		adcToCurrentValue *= 20;
		break;
	case 0b10:
		adcToCurrentValue *= 40;
		break;
	case 0b11:
		adcToCurrentValue *= 80;
		break;
	default:
		break;
	}
	adcToCurrentValue = 3.3/adcToCurrentValue;
}

void DRV8305::cariblation(){
	setAmpDCCariblation();
	uint32_t csValue[3] = {0,0,0};
	int readNum;
	readNum = 10;
	for(int i = 0;i<readNum;i++){
		csValue[0] += cs[0].read(ADC_SampleTime239);
		csValue[1] += cs[1].read(ADC_SampleTime239);
		csValue[2] += cs[2].read(ADC_SampleTime239);
	}
	adcOffset[0] = csValue[0] / readNum;
	adcOffset[1] = csValue[1] / readNum;
	adcOffset[2] = csValue[2] / readNum;
	setAmpNormalOperation();
}

float DRV8305::readCurrent(uint8_t phase){
	if(phase > 2)return 0;
	return (float)(adcValue[phase] - adcOffset[phase]) * adcToCurrentValue;
}
