#include "canNodeVoltage.h"

int16_t CanNodeVoltage::setup(SI8900 &si8900set,uint16_t portSet,float gainSet,CAN &canSet,uint16_t numberSet){
	this->can = &canSet;
	this->si8900 = &si8900set;
	port = portSet;				//SI8900のポート
	gain = gainSet;
	number = numberSet;			//設定用アドレス
	can->filterAdd(number + CAN_ADD_VOLTAGE_SETUP);

	interval = 0;

	System::cycleFunctionNumber++;

	return 0;
}


void CanNodeVoltage::cycle(){
	if(interval != 0){
    	if(intervalTimer <= millis()){
    		uint8_t canData[8];
    		intervalTimer = millis() + interval;

    		float_to_uchar4(canData,(float)si8900->value[port]*gain);

    		can->send(number + CAN_ADD_VOLTAGE_VALUE,4,canData);
    	}
	}
	System::cycleFunctionCnt--;
}

void CanNodeVoltage::interrupt(){
	if(can->rxMessage.StdId == number + CAN_ADD_VOLTAGE_SETUP){
		if(can->rxMessage.Data[0] == 1){
			interval = (can->rxMessage.Data[2] << 8) + can->rxMessage.Data[1];
			intervalTimer = millis();
		}
	}
	return;
}
