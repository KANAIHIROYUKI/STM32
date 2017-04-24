#include "canVoltage.h"


void CanVoltage::setup(CAN &canSet,uint16_t numberSet,uint16_t interval){
	this->can = &canSet;
	number = numberSet;	//エンコーダ値のアドレス
	can->filterAdd(number + CAN_ADD_VOLTAGE_VALUE);

	uint8_t canData[3];
	canData[0] = 1;
	canData[1] = interval & 0xFF;
	canData[2] = (interval >> 8) & 0xFF;
	can->send(CAN_ADD_VOLTAGE_SETUP + number,3,canData);	//インターバルの設定
	delay(1);
	receiveTime = millis();
}

int32_t CanVoltage::read(){
	return value;
}

void CanVoltage::interrupt(){
	if(can->rxMessage.StdId == number + CAN_ADD_VOLTAGE_VALUE){
		value = uchar4_to_int(can->rxMessage.Data);
		receiveTime = millis();
	}
}

uint64_t CanVoltage::lastReceiveTime(){
	return millis() - receiveTime;
}
