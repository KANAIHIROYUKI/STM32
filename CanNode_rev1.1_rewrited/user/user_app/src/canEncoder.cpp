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

	canEnc_wise = 1;
}

int64_t CanEncoder::read(){
	return canEnc_value * canEnc_wise;
}

float CanEncoder::velRead(){
	return canEnc_velocity;
}

void CanEncoder::interrupt(){
	if(canEnc_can->rxMessage.StdId == canEnc_address){
		canEnc_value = uchar4_to_int(canEnc_can->rxMessage.Data);
		uint8_t velData[4];
		velData[0] = canEnc_can->rxMessage.Data[4];
		velData[1] = canEnc_can->rxMessage.Data[5];
		velData[2] = canEnc_can->rxMessage.Data[6];
		velData[3] = canEnc_can->rxMessage.Data[7];
		canEnc_velocity = uchar4_to_float(velData);
		/*canEnc_value = canEnc_can->rxMessage.Data[0];
		canEnc_value += canEnc_can->rxMessage.Data[1]<<8;
		canEnc_value += canEnc_can->rxMessage.Data[2]<<16;
		canEnc_value += canEnc_can->rxMessage.Data[3]<<24;*/
		canEnc_receiveTime = millis();
	}
}

void CanEncoder::reset(){
	uint8_t canData[1];
	canData[0] = 0;
	canEnc_can->send(canEnc_address - 0x40,1,canData);
}

uint64_t CanEncoder::lastReceiveTime(){
	return millis() - canEnc_receiveTime;
}

void CanEncoder::reverse(int8_t wise){
	canEnc_wise = wise;
}