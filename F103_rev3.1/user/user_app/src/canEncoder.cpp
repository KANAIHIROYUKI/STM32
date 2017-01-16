#include "canEncoder.h"

void CanEncoder::setup(CAN &can,uint16_t number,uint16_t interval){
	this->canEnc_can = &can;
	canEnc_address = CAN_ADD_ENC_VALUE + number;	//エンコーダ値のアドレス
	canEnc_can->filterAdd(canEnc_address);

	uint8_t canData[] = {0,0,0,0,0,0,0,0};
	canData[0] = 1;
	canData[1] = interval;
	canEnc_can->send(CAN_ADD_ENC_SETUP + number,8,canData);	//インターバルの設定
	canEnc_receiveTime = millis();
}

uint32_t CanEncoder::read(){
	return canEnc_value;
}

void CanEncoder::interrupt(CanRxMsg rxMessage){
	if(rxMessage.StdId == canEnc_address){
		canEnc_value = uchar4_to_int(rxMessage.Data);
		canEnc_receiveTime = millis();
	}
}

void CanEncoder::reset(){
	uint8_t canData[] = {0,0,0,0,0,0,0,0};
	canData[0] = 0;
	canEnc_can->send(CAN_ADD_ENC_SETUP,8,canData);
}

uint64_t CanEncoder::lastReceiveTime(){
	return millis() - canEnc_receiveTime;
}
