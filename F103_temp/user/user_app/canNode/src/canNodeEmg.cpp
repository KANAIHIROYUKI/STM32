#include "canNodeEmg.h"

void CanNodeEmg::setup(CAN &_can,uint16_t myAddress){
	this->can = &_can;
	address = myAddress;
	//can->filterAdd(CAN_ADD_EMG);
}

void CanNodeEmg::emgRequest(uint16_t emgMessage){

	data[0] = emgMessage		& 0xff;
	data[1] = (emgMessage >> 8)	& 0xff;
	data[2] = address;
	can->send(CAN_ADD_EMG,3,data);
}

uint16_t CanNodeEmg::emgRead(){
	return 0;
}

void CanNodeEmg::interrupt(){
	if(can->rxMessage.StdId != CAN_ADD_EMG)return;

	emgStat = can->rxMessage.Data[0] + (can->rxMessage.Data[1] << 8);
}
