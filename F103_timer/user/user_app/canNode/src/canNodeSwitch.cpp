#include "canNodeSwitch.h"

void CanNodeSwitch::setup(Switch &_sw,CAN &_can,uint16_t canNumber){
	this->sw[0] = &_sw;
	this->can = &_can;
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
	System::cycleFunctionCnt--;

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
		send[0] = canData[0];
		send[1] = canData[1];
		lastSendTime = millis();
	}

	if(millis() - lastSendTime > intervalTime && intervalTime != 0){
		can->send(canAddress + 0x40,2,canData);
		send[0] = canData[0];
		send[1] = canData[1];
		lastSendTime = millis();
	}
}


void CanNodeSwitch::interrupt(){
	if(can->rxMessage.StdId == canAddress && can->rxMessage.Data[0] == 1){
		setuped++;
		intervalTime = (can->rxMessage.Data[2] << 8) + can->rxMessage.Data[1];
		lastSendTime = millis();
	}
}
