#include "AS504x.h"

void AS504x::setup(SPI_Master &spiSet,GPIO &ssSet){
	this->spi = &spiSet;
	spiif.setup(spiSet);
	spiif.nssAssign(ssSet);

	magencDeviceNumber = 1;
	valueReadCnt = 0;

	System::cycleFunctionNumber++;
}

void AS504x::addEnc(Enc &encSet){
	this->enc[magencDeviceNumber] = &encSet;
	magencDeviceNumber++;
}

uint64_t AS504x::read(uint16_t deviceNumberRead){
	if(deviceNumberRead > magencDeviceNumber)return 0;
	return countValue[deviceNumberRead];
}

void AS504x::cycle(){
	System::cycleFunctionCnt--;

	if(spiif.begin()){										//spi取得できると自動的にnssもアサートしてくれる
		delay(1);
		spi->changeMode(SPI_Mode3);
		for(int i=0;i<magencDeviceNumber*2;i++){			//空データ｡受信のみ､各16bitある
			spi->write(0);
		}
		spi->send();
	}

	if(spiif.available()){									//spi取得できている
		if(spi->available() >= magencDeviceNumber*2){		//送信が終わってデータ受信も終わった
			//delayMicros(100);
			valueReadCnt++;
			for(int i=0;i<magencDeviceNumber;i++){
				directlyValue[i] = ((spi->read() << 3) + (spi->read() >> 5)) & 0x3FF;

				if(oldDirectryValue[i] > directlyValue[i]){	//オーバーフロー/アンダーフロー処理														//oldのほうが大きい
					if(directlyValue[i] + 1023 - oldDirectryValue[i] > oldDirectryValue[i] - directlyValue[i]){	//なにもせず引き算して出したほうが差が小さい　=　飛んでいない
						countValue[i] += (int)(directlyValue[i] - oldDirectryValue[i]);
					}else{																							//0x1FF足した方が大きい　=　上から下に飛んでる
						countValue[i] += 1023 + directlyValue[i] - oldDirectryValue[i];
					}

				}else{																								//現在の値のほうが大きい
					if(oldDirectryValue[i] + 1023 - directlyValue[i] > directlyValue[i] - oldDirectryValue[i]){	//なにもせず引き算して出したほうが差が小さい　=　飛んでいない
						countValue[i] += (int)(directlyValue[i] - oldDirectryValue[i]);
					}else{																							//0x1FF足した方が大きい = 下から上に飛んでる
						countValue[i] += directlyValue[i] - (oldDirectryValue[i] + 1023);
					}
				}

				oldDirectryValue[i] = directlyValue[i];
			}

			spiif.end();
			delayMicros(100);
		}
	}
}

int AS504x::getValue(){
	uint16_t data[2];
	spi->changeMode(SPI_Mode3);

	//tClockFe = 0.5us
	spiif.nss->write(0);
	data[0] = spi->transfer(0);
	data[1] = spi->transfer(0);
	spiif.nss->write(1);
	return (int)((data[0] << 3) + (data[1] >> 5));
}

