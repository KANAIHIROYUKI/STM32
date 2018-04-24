#include "canNodeEncoder.h"

int16_t CanNodeEncoder::setup(TIM &enc,CAN &_can,uint16_t number){
	this->can = &_can;
	this->enc = &enc;
	address = CAN_ADD_ENC_SETUP + number;	//設定用アドレス
	can->filterAdd(address);

	System::cycleFunctionNumber++;

	return 0;
}


void CanNodeEncoder::cycle(){
	if(interval != 0){
    	if(intervalTimer <= millis()){
    		uint8_t canData[8];
    		intervalTimer = millis() + interval;

    		int_to_uchar4(canData,enc->read());

    		can->send(address + 0x40,4,canData);


    	}
	}
	System::cycleFunctionCnt--;
}

void CanNodeEncoder::interrupt(){
	if(can->rxMessage.StdId == address){
		if(can->rxMessage.Data[0] == 0){
			enc->reset();
		}else if(can->rxMessage.Data[0] == 1){
			interval = (can->rxMessage.Data[2] << 8) + can->rxMessage.Data[1];
			intervalTimer = millis();
		}
	}
	return;
}
