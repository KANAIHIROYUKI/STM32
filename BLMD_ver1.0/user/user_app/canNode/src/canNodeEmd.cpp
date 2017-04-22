#include "canNodeEmg.h"

void CanNodeEmg::setup(CAN &canSet){
	this->can = &canSet;
	can->filterAdd(0x001);
}

void CanNodeEmg::emgRequest(){
	data[0] = 0;
	can->send(0x000,2,data);
}

void CanNodeEmg::interrupt(){
	if(can->rxMessage.StdId == 0x001){
		if(can->rxMessage.Data[0] == 0){
			emgStat = 0;
		}else{
			emgStat = 1;
		}
	}
}
