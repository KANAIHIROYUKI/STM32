#include "canNodeEncoder.h"

int16_t CanNodeEncoder::setup(TIM &enc,CAN &can,uint16_t number){
	this->canEnc_can = &can;
	this->canEnc_enc = &enc;
	canEnc_address = CAN_ADD_ENC_SETUP + number;	//�ݒ�p�A�h���X
	canEnc_can->filterAdd(canEnc_address);

	rx.setup(can,canEnc_address);
	rx.attachFunc(interrupt);
	System::cycleFunctionNumber++;

	return 0;

}


void CanNodeEncoder::cycle(){


}


void CanNodeEncoder::interrupt(){


}


/*
int16_t CanNodeEncoder::setup(TIM &enc,CAN &can,uint16_t number){
	this->canEnc_can = &can;
	this->canEnc_enc = &enc;
	canEnc_address = CAN_ADD_ENC_SETUP + number;	//�ݒ�p�A�h���X
	canEnc_can->filterAdd(canEnc_address);

	System::cycleFunctionNumber++;

	return 0;
}


void CanNodeEncoder::cycle(){
	System::cycleFunctionCnt--;

	if(canEnc_interval != 0){
    	if(canEnc_intervalTimer <= millis()){
    		uint8_t canData[8];
    		canEnc_intervalTimer = millis() + canEnc_interval;

    		int_to_uchar4(canData,canEnc_enc->read());

    		canEnc_can->send(canEnc_address + 0x40,4,canData);


    	}
	}
}

void CanNodeEncoder::interrupt(){
	if(canEnc_can->rxMessage.StdId == canEnc_address){
		if(canEnc_can->rxMessage.Data[0] == 0){
			canEnc_enc->reset();
		}else if(canEnc_can->rxMessage.Data[0] == 1){
			canEnc_interval = (canEnc_can->rxMessage.Data[2] << 8) + canEnc_can->rxMessage.Data[1];
			canEnc_intervalTimer = millis();
		}
	}
	return;
}
*/