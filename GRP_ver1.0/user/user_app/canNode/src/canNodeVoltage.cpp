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
	System::cycleFunctionCnt--;

	if(si8900->readStat[port]){
		voltageValue = (float)si8900->read(port)*gain;
		vAve.stack(voltageValue);
	}

	if(interval != 0){
    	if(intervalTimer <= millis()){
    		uint8_t canData[8];
    		intervalTimer = millis() + interval;

    		float_to_uchar4(canData,vAve.read());

    		can->send(number + CAN_ADD_VOLTAGE_VALUE,4,canData);
    	}
	}
}

void CanNodeVoltage::interrupt(){
	if((uint16_t)(can->rxMessage.StdId) == (number + CAN_ADD_VOLTAGE_SETUP)){
		if(can->rxMessage.Data[0] == 1){
			interval = (can->rxMessage.Data[2] << 8) + can->rxMessage.Data[1];
			intervalTimer = millis();
		}
	}
	return;
}
