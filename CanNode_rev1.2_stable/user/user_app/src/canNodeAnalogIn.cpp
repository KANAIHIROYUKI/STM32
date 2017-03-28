#include "canNodeAnalogIn.h"

int16_t CanNodeAnalog::setup(ADC &adc,CAN &can,uint16_t number){
	this->canNodeAnalog_can = &can;
	this->canNodeAnalog_adc = &adc;
	canNodeAnalog_address = CAN_ADD_ENC_SETUP + number;

	canNodeAnalog_interval = 0;

	return 0;
}


void CanNodeAnalog::cycle(){
	if(canNodeAnalog_interval != 0){
    	if(canNodeAnalog_intervalTimer <= millis()){
    		uint8_t canData[8];
    		canNodeAnalog_intervalTimer = millis() + canNodeAnalog_interval;

    		int_to_uchar4(canData,canNodeAnalog_adc->read());

    		canNodeAnalog_can->send(canNodeAnalog_address + 0x40,4,canData);	//設定用0x400,データ用0x440なので足す
    	}
	}
}

void CanNodeAnalog::interrupt(){
	if(canNodeAnalog_can->rxMessage.StdId == canNodeAnalog_address){
		if(canNodeAnalog_can->rxMessage.Data[0] == 0){
			//エンコーダならリセット
		}else if(canNodeAnalog_can->rxMessage.Data[0] == 1){
			canNodeAnalog_interval = (canNodeAnalog_can->rxMessage.Data[2] << 8) + canNodeAnalog_can->rxMessage.Data[1];
			canNodeAnalog_intervalTimer = millis();
		}
	}
}
