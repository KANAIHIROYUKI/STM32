#include "AS504x.h"

void AS504x::setup(SPI_Master &spiSet,GPIO &ssSet){
	this->spi = &spiSet;
	spiif.setup(spiSet);
	spiif.nssAssign(ssSet);

	magencDeviceNumber++;

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

	if(spiif.begin()){										//spi�擾�ł���Ǝ����I��nss���A�T�[�g���Ă����
		for(int i=0;i<magencDeviceNumber*2;i++){			//��f�[�^���M�̂ݤ�e16bit����
			spi->write(0);
		}
	}

	if(spiif.available()){									//spi�擾�ł��Ă���
		if(spi->available() == magencDeviceNumber*2){		//���M���I����ăf�[�^��M���I�����

			for(int i=0;i<magencDeviceNumber;i++){
				directlyValue[i] = (spi->read() << 8) + spi->read();

				if(oldDirectryValue[i] > directlyValue[i]){	//�I�[�o�[�t���[/�A���_�[�t���[����														//old�̂ق����傫��
					if(directlyValue[i] + 0x1FF - oldDirectryValue[i] > oldDirectryValue[i] - directlyValue[i]){	//�Ȃɂ����������Z���ďo�����ق��������������@=�@���ł��Ȃ�
						countValue[i] += (int)(directlyValue[i] - oldDirectryValue[i]);
					}else{																							//0x1FF�����������傫���@=�@�ォ�牺�ɔ��ł�
						countValue[i] += 0x1FF + directlyValue[i] - oldDirectryValue[i];
					}

				}else{																								//���݂̒l�̂ق����傫��
					if(oldDirectryValue[i] + 0x1FF - directlyValue[i] > directlyValue[i] - oldDirectryValue[i]){	//�Ȃɂ����������Z���ďo�����ق��������������@=�@���ł��Ȃ�
						countValue[i] += (int)(directlyValue[i] - oldDirectryValue[i]);
					}else{																							//0x1FF�����������傫�� = �������ɔ��ł�
						countValue[i] += directlyValue[i] - (oldDirectryValue[i] + 0x1FF);
					}
				}

				oldDirectryValue[i] = directlyValue[i];
			}

			spiif.end();
		}
	}
}

