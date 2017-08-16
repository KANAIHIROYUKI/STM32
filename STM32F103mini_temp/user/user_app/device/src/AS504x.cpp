#include "AS504x.h"


void AS504x::setup(SPI_Master &spiSet,GPIO &ssSet){
	this->spi = &spiSet;
	this->ss  = &ssSet;

	spiInterfaceNumber = spi->getInterfaceNumber();

	deviceNumber = 0;
}

void AS504x::addEnc(Enc &encSet){
	this->enc[deviceNumber] = &encSet;
	deviceNumber++;
}

uint64_t AS504x::read(uint16_t deviceNumberRead){
	if(deviceNumberRead > deviceNumber)return 0;
	return countValue[deviceNumberRead];
}

void AS504x::cycle(){
	for(int i=0;i<deviceNumber;i++){
		enc[i]->write(read(i));
	}


	if(spi->takeInterfaceNumber != 0){								//自分がspi使っている時
		if(spi->takeInterfaceNumber == spiInterfaceNumber){
			if(spi->available() >= deviceNumber * 2){
				ss->write(1);
				spi->releace();

				for(int i=0;i<deviceNumber;i++){
					directlyValue[i] = spi->read() << 2;
					directlyValue[i] |= spi->read() >> 6;
				}

			}else{													//spiで通信(受信)中
				if(millis() - spiStartTime > AS5040_SPI_TIMEOUT){	//タイムアウト
					ss->write(1);
					spi->releace();
				}
			}
		}else{	//他のインターフェイスがSPIを使用中

		}
	}else{		//どこもspiを使用していない
		ss->write(0);

		spiStartTime = millis();
		spi->take(spiInterfaceNumber);			//spi使用､バッファクリア
		for(int i=0;i<deviceNumber*2;i++){
			//spi->write(0);					//送信しても意味が無いので全部ゼロ
			spi->write(i);						//ループバックテスト用
		}
	}
}
