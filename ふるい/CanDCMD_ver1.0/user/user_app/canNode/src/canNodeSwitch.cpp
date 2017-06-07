#include "canNodeSwitch.h"

void CanNodeSwitch::setup(Switch &sw_,CAN &can_,uint16_t canNumber){
	this->sw[0] = &sw_;
	this->can = &can_;
	canAddress = CAN_ADD_SWITCH_SETUP + canNumber;
	can->filterAdd(canAddress);
	setuped = 0;

	switchNumber = 1;
	System::cycleFunctionNumber++;

	cycle();
}

void CanNodeSwitch::pinAdd(Switch &sw){
	if(switchNumber < 8)this->sw[switchNumber] = &sw;
	switchNumber++;
}

void CanNodeSwitch::cycle(){
	canData[0] = 0;
	canData[1] = 0;

	for(int i=0;i<switchNumber;i++){
		sw[i]->cycle();
	}

	for(int i=0;i<switchNumber;i++){
		canData[0] |= sw[i]->readStat << i;
		canData[1] |= sw[i]->read() << i;
	}

	if(canData[0] != 0 && intervalTime != 0){
		can->send(canAddress + 0x40,2,canData);
		lastSendTime = millis();
	}

	if(millis() - lastSendTime > intervalTime && intervalTime != 0){
		can->send(canAddress + 0x40,2,canData);
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
