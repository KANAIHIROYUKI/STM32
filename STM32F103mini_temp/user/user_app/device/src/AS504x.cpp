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


	if(spi->takeInterfaceNumber != 0){								//������spi�g���Ă��鎞
		if(spi->takeInterfaceNumber == spiInterfaceNumber){
			if(spi->available() >= deviceNumber * 2){
				ss->write(1);
				spi->releace();

				for(int i=0;i<deviceNumber;i++){
					directlyValue[i] = spi->read() << 2;
					directlyValue[i] |= spi->read() >> 6;
				}

			}else{													//spi�ŒʐM(��M)��
				if(millis() - spiStartTime > AS5040_SPI_TIMEOUT){	//�^�C���A�E�g
					ss->write(1);
					spi->releace();
				}
			}
		}else{	//���̃C���^�[�t�F�C�X��SPI���g�p��

		}
	}else{		//�ǂ���spi���g�p���Ă��Ȃ�
		ss->write(0);

		spiStartTime = millis();
		spi->take(spiInterfaceNumber);			//spi�g�p��o�b�t�@�N���A
		for(int i=0;i<deviceNumber*2;i++){
			//spi->write(0);					//���M���Ă��Ӗ��������̂őS���[��
			spi->write(i);						//���[�v�o�b�N�e�X�g�p
		}
	}
}
