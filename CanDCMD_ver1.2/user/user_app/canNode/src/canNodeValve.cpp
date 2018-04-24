#include "canNodeValve.h"

int CanNodeValve::setup(CAN &_can,uint16_t number){
	this->can = &_can;
	address = CAN_ADD_VALVE + number;
	pinNumber = 0;

	can->filterAdd(address);
	return 0;
}

void CanNodeValve::pinAdd(GPIO &_gpio){
	this->gpio[pinNumber] = &_gpio;
	pinNumber++;
}


void CanNodeValve::interrupt(){
	if(can->rxMessage.StdId == address){

		for(int i=0;i<pinNumber;i++){
			if((can->rxMessage.Data[0] & (1 << i)) != 0){
				if(((can->rxMessage.Data[1] & (1 << i)) != 0)){
					gpio[i]->write(Bit_SET);
				}else{
					gpio[i]->write(Bit_RESET);
				}
			}
		}
	}
}
