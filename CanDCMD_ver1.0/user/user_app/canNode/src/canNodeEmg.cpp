#include "canNodeEmg.h"

void CanNodeEmg::setup(CAN &canSet){
	this->can = &canSet;
	can->filterAdd(CAN_ADD_EMG);
}

void CanNodeEmg::emgRequest(uint16_t emgMessage){
	data[0] = 0xF & emgMessage;
	data[1] = 0xF & (emgMessage >> 8);
	can->send(CAN_ADD_EMG,2,data);
}

void CanNodeEmg::interrupt(){
	if(can->rxMessage.StdId == CAN_ADD_EMG){
		if(can->rxMessage.Data[0] == 0){
			emgStat = 0;
		}else{
			emgStat = 1;
		}
	}
}
