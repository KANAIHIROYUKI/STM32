#include "canEncoder.h"

void CanEncoder::setup(CAN &can,uint16_t number,uint16_t interval){
	this->canEnc_can = &can;
	canEnc_address = CAN_ADD_ENC_VALUE + number;	//�G���R�[�_�l�̃A�h���X
	canEnc_can->filterAdd(canEnc_address);

	uint8_t canData[3];
	canData[0] = 1;
	canData[1] = interval & 0xFF;
	canData[2] = (interval >> 8) & 0xFF;
	canEnc_can->send(CAN_ADD_ENC_SETUP + number,3,canData);	//�C���^�[�o���̐ݒ�
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
	uint8_t canData[1];
	canData[0] = 0;
	canEnc_can->send(CAN_ADD_ENC_SETUP,1,canData);
}

uint64_t CanEncoder::lastReceiveTime(){
	return millis() - canEnc_receiveTime;
}
