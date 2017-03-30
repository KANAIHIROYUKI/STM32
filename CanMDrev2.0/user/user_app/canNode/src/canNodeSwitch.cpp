#include "canNodeSwitch.h"

void CanNodeSwitch::setup(Switch &sw_,CAN &can_,uint16_t canNumber){
	this->sw = &sw_;
	this->can = &can_;
	canAddress = CAN_ADD_SWITCH_SETUP + canNumber;
	can->filterAdd(canAddress);
	setuped = 0;

	System::cycleFunctionNumber++;
}

void CanNodeSwitch::cycle(){

	if(sw->readStat && intervalTime != 0){
		canData[0] = sw->read();
		canData[1] = 0;
		canData[2] = 0;
		canData[3] = 0;
		can->send(canAddress + 0x40,4,canData);
		lastSendTime = millis();
	}

	if(millis() - lastSendTime > intervalTime && intervalTime != 0){
		canData[0] = sw->read();
		canData[1] = 0;
		canData[2] = 0;
		canData[3] = 0;
		can->send(canAddress + 0x40,4,canData);
		lastSendTime = millis();
	}

	System::cycleFunctionCnt--;
}


void CanNodeSwitch::interrupt(){
	if(can->rxMessage.StdId == canAddress && can->rxMessage.Data[0] == 1){
		setuped++;
		intervalTime = (can->rxMessage.Data[2] << 8) + can->rxMessage.Data[1];
		lastSendTime = millis();
	}
}
