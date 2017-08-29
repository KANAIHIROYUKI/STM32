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


uint16_t SPI_Interface::begin(){			//��{�I�ɑS�ẴC���^�[�t�F�C�X��cycle�ŉ���Ă���͂��Ȃ̂Ť���ԂɂȂ�͂�
	if(takeRequested == 0)return 0;			//����spiif���g�p��

	if(spi->take(interfaceNumber)){			//take()�͎擾����"������"1�ɂȂ�
		takeRequested = 0;

		if(nssSetuped){
			nss->write(0);	//nss�̑��������
		}

		return 1;

	}else{									//����spi�擾���Ă�
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
