#include "canNodeValve.h"

int CanNodeValve::setup(CAN &can,uint16_t address){
	this->canValve_can = &can;
	canValve_address = address;
	canValuve_pinNumber = 0;

	canValve_can->filterAdd(canValve_address);

	return 0;
}

void CanNodeValve::pinAdd(GPIO &gpio){
	this->canValve_gpio[canValuve_pinNumber] = &gpio;
	canValuve_pinNumber++;
}


void CanNodeValve::interrupt(CanRxMsg rxMessage){
	if(rxMessage.StdId == canValve_address){
		for(int i=0;i<canValuve_pinNumber;i++){
			if((rxMessage.Data[0] & (1 << i)) != 0){
				if(((rxMessage.Data[1] & (1 << i)) != 0)){
					canValve_gpio[i]->write(Bit_SET);
				}else{
					canValve_gpio[i]->write(Bit_RESET);
				}
			}
		}
	}
}
