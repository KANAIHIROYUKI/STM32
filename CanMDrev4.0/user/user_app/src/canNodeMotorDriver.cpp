#include "canNodeMotorDriver.h"

int16_t CanNodeMotorDriver::setup(MOTOR &motor,CAN &can,uint16_t number,uint16_t maxInterval){
	this->canMd_motor = &motor;
	this->canMd_can = &can;
	canMd_address[CAN_MD_ADDRESS_DUTY] = CAN_ADD_DUTY + number;
	canMd_address[CAN_MD_ADDRESS_FREE] = CAN_ADD_FREE + number;

	canMd_can->filterAdd(canMd_address[CAN_MD_ADDRESS_DUTY],canMd_address[CAN_MD_ADDRESS_FREE]);

	canMd_receiveTime = 0;
	canMd_maxInterval = maxInterval;

	ledAssigned = 0;
	return 0;
}

void CanNodeMotorDriver::ledAssign(GPIO &led){
	this->canMd_led = &led;
	ledAssigned = 1;
}

void CanNodeMotorDriver::cycle(){
	if(canMd_receiveTime + canMd_maxInterval < millis()){
		canMd_motor->duty(0);
		if(ledAssigned && (canMd_ledTime < millis())){
			canMd_led->toggle();
			canMd_ledTime = millis() + LED_INTERVAL_NORECEIVE;
		}
	}else{
		if(ledAssigned && (canMd_ledTime < millis())){
			canMd_led->toggle();
			canMd_ledTime = millis() + LED_INTERVAL_RECEIVE;
		}
	}
}

void CanNodeMotorDriver::interrupt(){
	if(canMd_can->rxMessage.StdId == canMd_address[CAN_MD_ADDRESS_DUTY]){
		outDuty = ((canMd_can->rxMessage.Data[1] & 0b01111111) << 8) | canMd_can->rxMessage.Data[0];

		if((canMd_can->rxMessage.Data[1] >> 7) == 1){
			outDuty = -outDuty;
		}
		canMd_motor->duty((float)outDuty/32767.0f);
		canMd_receiveTime = millis();
	}else if(canMd_can->rxMessage.StdId == canMd_address[CAN_MD_ADDRESS_FREE]){
		outDuty = (canMd_can->rxMessage.Data[1] << 8) | canMd_can->rxMessage.Data[0];

		canMd_motor->brake((float)outDuty/32767.0f);
		canMd_receiveTime = millis();
	}
}
