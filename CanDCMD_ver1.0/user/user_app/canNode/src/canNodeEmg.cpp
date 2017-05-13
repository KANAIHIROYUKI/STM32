#include "canNodeEmg.h"

void CanNodeEmg::setup(CAN &canSet){
	this->can = &canSet;
}

void CanNodeEmg::emgRequest(uint16_t emgMessage){
	data[0] = emgMessage		& 0xf;
	data[1] = (emgMessage >> 8)	& 0xf;
	can->send(CAN_ADD_EMG,2,data);
}
