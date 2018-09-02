#include "canEncoder.h"

void CanEncoder::setup(CAN &_can,uint16_t number,uint16_t interval){
	this->can = &_can;
	address = CAN_ADD_ENC_VALUE + number;	//エンコーダ値のアドレス
	can->filterAdd(address);

	uint8_t canData[3];
	canData[0] = 1;
	canData[1] = interval & 0xFF;
	canData[2] = (interval >> 8) & 0xFF;
	can->send(CAN_ADD_ENC_SETUP + number,3,canData);	//インターバルの設定
	delay(1);
	receiveTime = millis();

	wise = 1;
}

int32_t CanEncoder::read(){
	return value * wise;
}

void CanEncoder::interrupt(){
	if(can->rxMessage.StdId == address){
		value = uchar4_to_int(can->rxMessage.Data);

		/*canEnc_value = canEnc_can->rxMessage.Data[0];
		canEnc_value += canEnc_can->rxMessage.Data[1]<<8;
		canEnc_value += canEnc_can->rxMessage.Data[2]<<16;
		canEnc_value += canEnc_can->rxMessage.Data[3]<<24;*/
		receiveTime = millis();
	}
}

void CanEncoder::reset(){
	uint8_t canData[1];
	canData[0] = 0;
	can->send(address - 0x40,1,canData);
}

uint64_t CanEncoder::lastReceiveTime(){
	return millis() - receiveTime;
}

void CanEncoder::reverse(int8_t wise){
	wise = wise;
}
