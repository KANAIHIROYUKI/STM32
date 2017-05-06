#include "canNodeMotorDriver.h"

int16_t CanNodeMotorDriver::setup(Motor &motor,CAN &can,uint16_t number,uint16_t max_interval){
	this->canMd_motor = &motor;
	this->canMd_can = &can;
	canMd_address[CAN_MD_ADDRESS_DUTY] = CAN_ADD_DUTY + number;
	canMd_address[CAN_MD_ADDRESS_FREE] = CAN_ADD_FREE + number;

	canMd_can->filterAdd(canMd_address[CAN_MD_ADDRESS_DUTY],canMd_address[CAN_MD_ADDRESS_FREE]);

	lastReceiveTime = 0;
	maxInterval = max_interval;

	ledAssigned = 0;

	System::cycleFunctionNumber++;

	motorOutEnable = 1;

	return 0;
}

void CanNodeMotorDriver::ledAssign(GPIO &led){
	this->canMd_led = &led;
	ledAssigned = 1;
}

void CanNodeMotorDriver::cycle(){
	if(lastReceiveTime + maxInterval < millis()){

		if(motorOutEnable == 1)canMd_motor->duty(0);							//outEnじゃないと回さんで

		if(ledAssigned == 1 && (ledTime < millis())){
			canMd_led->toggle();
			ledTime = millis() + LED_INTERVAL_NORECEIVE;
		}
	}else{
		if(ledAssigned == 1 && (ledTime < millis())){
			canMd_led->toggle();
			ledTime = millis() + LED_INTERVAL_RECEIVE;
		}
	}
	System::cycleFunctionCnt--;
}

void CanNodeMotorDriver::interrupt(){
	if(canMd_can->rxMessage.StdId == canMd_address[CAN_MD_ADDRESS_DUTY]){
		outDuty = ((canMd_can->rxMessage.Data[1] & 0b01111111) << 8) | canMd_can->rxMessage.Data[0];

		if((canMd_can->rxMessage.Data[1] >> 7) == 1){
			outDuty = -outDuty;
		}
		if(motorOutEnable == 1)canMd_motor->duty((float)outDuty/32767);			//outEnじゃないと回さんで
		lastReceiveTime = millis();
	}else if(canMd_can->rxMessage.StdId == canMd_address[CAN_MD_ADDRESS_FREE]){

		if(motorOutEnable == 1)canMd_motor->free();								//outEnじゃないと回さんで

		lastReceiveTime = millis();
	}
}

void CanNodeMotorDriver::motorOverRide(uint16_t overRideEn){
	if(overRideEn == 0){						//オーバーライドおしまい
		motorOutEnable = 1;
		canMd_motor->free();
	}

	if(overRideEn == 1){
		motorOutEnable = 2;
	}
}

void CanNodeMotorDriver::ledOverRide(uint16_t overRideEn){
	if(ledAssigned == 1 && overRideEn != 0){
		ledAssigned = 2;
	}else if(ledAssigned == 2 && overRideEn == 0){
		ledAssigned = 1;
	}
}
