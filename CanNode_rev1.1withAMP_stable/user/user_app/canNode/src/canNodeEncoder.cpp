#include "canNodeEncoder.h"

int16_t CanNodeEncoder::setup(TIM &enc,CAN &can,uint16_t number){
	this->canEnc_can = &can;
	this->canEnc_enc = &enc;
	canEnc_address = CAN_ADD_ENC_SETUP + number;	//設定用アドレス
	canEnc_can->filterAdd(canEnc_address);

	System::cycleFunctionNumber++;

	canEnc_oldCnt = canEnc_enc->read();
	return 0;
}


void CanNodeEncoder::cycle(){
	if(canEnc_interval != 0){
    	if(canEnc_intervalTimer <= millis()){
    		uint8_t canData[8],velData[4];

    		canEnc_intervalTimer = millis() + canEnc_interval;

    		float_to_uchar4(velData,(float)(canEnc_enc->read() - canEnc_oldCnt)/canEnc_interval);
    		canEnc_oldCnt = canEnc_enc->read();

    		int_to_uchar4(canData,canEnc_enc->read());

    		canData[4] = velData[0];
    		canData[5] = velData[1];
    		canData[6] = velData[2];
    		canData[7] = velData[3];

    		canEnc_can->send(canEnc_address + 0x40,4,canData);
    		//canEnc_can->send(canEnc_address + 0x40,8,canData);
    		//canEnc_can->send(0x444,8,canData);
    	}
	}
	System::cycleFunctionCnt--;
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
