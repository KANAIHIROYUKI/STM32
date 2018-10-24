#include "canInterface.h"

/*
void CAN_RX::setup(CAN *can,uint16_t _address,uint64_t _timeout){
	this->can = can;
	address = _address;
	timeout = _timeout;
}

int CAN_RX::isReceived(){
	return 1;
}

void CAN_RX::interrupt(){
	if(can->rxMessage.StdId == address){
		input();
		lastReceiveTime = millis();
	}
}

void CAN_RX::input(){
	data = uchar4_to_int(can->rxMessage.Data);
}*/
