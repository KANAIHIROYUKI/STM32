#include "canNodeEncoder.h"

int16_t CanNodeEncoder::setup(TIM &enc,CAN &can,uint16_t address){
	this->canEnc_can = &can;
	this->canEnc_enc = &enc;
	canEnc_address = address;
	canEnc_can->filterAdd(canEnc_address);
	return 0;
}


void CanNodeEncoder::cycle(){
	if(canEnc_intervalTimer != 0){
    	if(canEnc_intervalTimer < millis()){
    		uint8_t canData[8];
    		uint32_t encValue = canEnc_enc->read();
    		canEnc_intervalTimer = millis() + canEnc_interval;

    		canData[0] = encValue & 0xFF;
    		canData[1] = (encValue >> 8) & 0xFF;
    		canData[2] = (encValue >> 16) & 0xFF;
    		canData[3] = (encValue >> 24) & 0xFF;
    		canEnc_can->send(canEnc_address,4,canData);
    	}
	}
}

void CanNodeEncoder::interrupt(CanRxMsg rxMessage){
	if(rxMessage.StdId == canEnc_address){
		if(rxMessage.Data[0] == 0){
			canEnc_enc->reset();
		}else if(rxMessage.Data[0] == 1){
			canEnc_interval = (rxMessage.Data[2] << 8) + rxMessage.Data[1];
		}
	}
	return;
}
