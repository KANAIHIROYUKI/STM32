#include "canNodeValve.h"

int CanNodeValve::setup(GPIO &gpio,uint8_t pinNumber,CAN &can,uint16_t address){
	this->canValve_can = &can;
	this->canValve_gpio = &gpio;
	canValve_address = address;
	return 0;
}


void CanNodeValve::interrupt(CanRxMsg rxMessage){
	if(rxMessage.StdId == canValve_address){
		for(int i=0;i<8;i++){
			if((rxMessage.Data[0] & (1 << i)) != 0){
				if(((rxMessage.Data[1] & (1 << i)) != 0)){
					canValve_port->io[i].write(Bit_SET);
				}else{
					canValve_port->io[i].write(Bit_RESET);
				}
			}
		}
	}
}
