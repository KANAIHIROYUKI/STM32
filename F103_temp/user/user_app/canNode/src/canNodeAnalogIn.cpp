#include "canNodeAnalogIn.h"

int16_t CanNodeAnalog::setup(ADC &_adc,CAN &_can,uint16_t number){
	this->can = &_can;
	this->adc = &_adc;
	address = CAN_ADD_ENC_SETUP + number;

	interval = 0;

	System::cycleFunctionNumber++;

	return 0;
}


void CanNodeAnalog::cycle(){
	System::cycleFunctionCnt--;

	if(interval != 0){
    	if(intervalTimer <= millis()){
    		uint8_t canData[8];
    		intervalTimer = millis() + interval;

    		int_to_uchar4(canData,adc->read());

    		can->send(address + 0x40,4,canData);	//�ݒ�p0x400,�f�[�^�p0x440�Ȃ̂ő���
    	}
	}
}

void CanNodeAnalog::interrupt(){
	if(can->rxMessage.StdId == address){
		if(can->rxMessage.Data[0] == 0){
			//�G���R�[�_�Ȃ烊�Z�b�g
		}else if(can->rxMessage.Data[0] == 1){
			interval = (can->rxMessage.Data[2] << 8) + can->rxMessage.Data[1];
			intervalTimer = millis();
		}
	}
}
