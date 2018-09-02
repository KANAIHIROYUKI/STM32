#include "canNodeValve.h"

int CanNodeValve::setup(CAN &can,uint16_t number){
	this->canValve_can = &can;
	canValve_address = CAN_ADD_VALVE + number;
	canValuve_pinNumber = 0;

	canValve_can->filterAdd(canValve_address);
	pinOut = 0;
	return 0;
}

void CanNodeValve::pinAdd(GPIO &gpio){
	this->canValve_gpio[canValuve_pinNumber] = &gpio;
	canValuve_pinNumber++;
}


void CanNodeValve::interrupt(){
	if(canValve_can->rxMessage.StdId == canValve_address){

		for(int i=0;i<canValuve_pinNumber;i++){
			if((canValve_can->rxMessage.Data[0] & (1 << i)) != 0){
				if(((canValve_can->rxMessage.Data[1] & (1 << i)) != 0)){
					canValve_gpio[i]->write(Bit_SET);
					pinOut |= (1<<i);

				}else{
					canValve_gpio[i]->write(Bit_RESET);
					pinOut &= 0xFF - (1<<i);
				}
			}
		}
	}
}
