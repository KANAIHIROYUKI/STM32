#include "canNodeEmg.h"

void CanNodeEmg::setup(CAN &canSet){
	this->can = &canSet;
	//can->filterAdd(CAN_ADD_EMG);
}

void CanNodeEmg::emgRequest(uint16_t emgMessage){

	data[0] = emgMessage		& 0xff;
	data[1] = (emgMessage >> 8)	& 0xff;
	can->send(CAN_ADD_EMG,2,data);
}

uint16_t CanNodeEmg::emgRead(){
	return 0;
}

void CanNodeEmg::interrupt(){
	if(can->rxMessage.StdId != CAN_ADD_EMG)return;

	emgStat = can->rxMessage.Data[0] + (can->rxMessage.Data[1] << 8);
}
