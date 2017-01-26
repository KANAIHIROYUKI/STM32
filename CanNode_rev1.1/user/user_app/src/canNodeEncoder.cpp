#include "canNodeEncoder.h"

int16_t CanNodeEncoder::setup(TIM &enc,CAN &can,uint16_t number){
	this->canEnc_can = &can;
	this->canEnc_enc = &enc;
	canEnc_address = CAN_ADD_ENC_SETUP + number;	//設定用アドレス
	canEnc_can->filterAdd(canEnc_address);

	canEnc_intervalTimer = millis();// + 100;

	return 0;
}


void CanNodeEncoder::cycle(){
	if(canEnc_interval != 0){
    	if(canEnc_intervalTimer < millis()){
    		uint8_t canData[8];
    		uint32_t encValue = canEnc_enc->read();
    		canEnc_intervalTimer = millis() + canEnc_interval;

    		int_to_uchar4(canData,encValue);

    		/*canData[0] = encValue & 0xFF;
    		canData[1] = (encValue >> 8) & 0xFF;
    		canData[2] = (encValue >> 16) & 0xFF;
    		canData[3] = (encValue >> 24) & 0xFF;*/
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
		}
	}
	return;
}
