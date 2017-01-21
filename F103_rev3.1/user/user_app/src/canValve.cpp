#include "canValve.h"

void CanValve::setup(CAN &can,uint16_t number){
	this->canValve_can = &can;
	canValve_address = CAN_ADD_VALVE + number;	//エンコーダ値のアドレス
	canValve_can->filterAdd(canValve_address);
}
void CanValve::write(uint8_t pin,uint8_t stat){
	uint8_t canData[2] = {0,0};
	canData[0] = 1 << pin;
	canData[1] = stat << pin;
	canValve_can->send(CAN_ADD_VALVE,2,canData);
}
