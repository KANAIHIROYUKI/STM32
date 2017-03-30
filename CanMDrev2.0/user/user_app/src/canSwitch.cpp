#include "canSwitch.h"

void CanSwitch::setup(CAN &can_,uint16_t number,uint32_t interval_time){
	this->can = &can_;
	canAddress = number + CAN_ADD_SWITCH_VALUE;

	uint8_t canData[3];
	canData[0] = 1;
	canData[1] = interval_time & 0xFF;
	canData[2] = (interval_time >> 8) & 0xFF;
	can->send(CAN_ADD_SWITCH_SETUP + number,3,canData);	//インターバルの設定

	can->filterAdd(canAddress);
	delay(1);

	//switchValue = 0;
	lastReceiveTime = 0;
}

uint16_t CanSwitch::read(){
	return switchValue;
}

void CanSwitch::interrupt(){
	if(can->rxMessage.StdId == canAddress){
		switchValue = can->rxMessage.Data[0];
		lastReceiveTime = millis();
	}
}
