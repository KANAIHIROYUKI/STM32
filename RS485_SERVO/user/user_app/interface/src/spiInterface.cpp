#include "spiInterface.h"

void SPI_Interface::setup(SPI_Master &spiSet){
	this->spi = &spiSet;
	interfaceNumber = spi->getInterfaceNumber();

	nssSetuped = 0;
}

void SPI_Interface::nssAssign(GPIO &nssSet){
	this->nss = &nssSet;
	nss->write(1);

	nssSetuped = 1;
}


uint16_t SPI_Interface::begin(){			//基本的に全てのインターフェイスがcycleで回っているはずなので､順番になるはず
	if(takeRequested == 0)return 0;			//他のspiifが使用中

	if(spi->take(interfaceNumber)){			//take()は取得した"時だけ"1になる｡
		takeRequested = 0;

		if(nssSetuped){
			nss->write(0);	//nssの操作怪しい
		}

		return 1;

	}else{									//既にspi取得してる
		return 0;
	}
}

void SPI_Interface::end(){
	spi->releace(interfaceNumber);

	if(nssSetuped){
		nss->write(1);
	}
}

uint16_t SPI_Interface::available(){
	if(spi->takeInterfaceNumber == interfaceNumber){
		return 1;
	}else{
		return 0;
	}
}
