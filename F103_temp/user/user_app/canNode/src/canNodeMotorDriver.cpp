#include "canNodeMotorDriver.h"

int16_t CanNodeMotorDriver::setup(Motor &_motor,CAN &_can,uint16_t number,uint16_t max_interval){
	this->motor = &_motor;
	this->can = &_can;
	address[CAN_MD_ADDRESS_DUTY] = CAN_ADD_DUTY + number;
	address[CAN_MD_ADDRESS_FREE] = CAN_ADD_FREE + number;

	can->filterAdd(address[CAN_MD_ADDRESS_DUTY],address[CAN_MD_ADDRESS_FREE]);

	lastReceiveTime = 0;
	maxInterval = max_interval;

	ledAssigned = 0;

	System::cycleFunctionNumber++;

	motorOutEnable = 1;

	outDuty = 0;
	outDutyF = 0;

	return 0;
}

void CanNodeMotorDriver::ledAssign(GPIO &_led){
	this->led = &_led;
	ledAssigned = 1;
}

void CanNodeMotorDriver::cycle(){
	System::cycleFunctionCnt--;

	if(lastReceiveTime + maxInterval < millis()){

		if(motorOutEnable == 1)motor->duty(0);							//outEnじゃないと回さんで

		if(ledAssigned == 1 && (ledTime < millis())){
			led->toggle();
			ledTime = millis() + LED_INTERVAL_NORECEIVE;
		}
	}else{
		if(ledAssigned == 1 && (ledTime < millis())){
			led->toggle();
			ledTime = millis() + LED_INTERVAL_RECEIVE;
		}
	}
}

void CanNodeMotorDriver::interrupt(){
	if(can->rxMessage.StdId == address[CAN_MD_ADDRESS_DUTY]){
		outDuty = ((can->rxMessage.Data[1] & 0b01111111) << 8) | can->rxMessage.Data[0];

		if((can->rxMessage.Data[1] >> 7) == 1){
			outDuty = -outDuty;
		}
		outDutyF = (float)outDuty/32767;
		if(motorOutEnable == 1)motor->duty(outDutyF);			//outEnじゃないと回さんで
		lastReceiveTime = millis();
	}else if(can->rxMessage.StdId == address[CAN_MD_ADDRESS_FREE]){

		if(motorOutEnable == 1)motor->free();								//outEnじゃないと回さんで

		lastReceiveTime = millis();
	}
}

void CanNodeMotorDriver::motorOverRide(uint16_t overRideEn){
	if(overRideEn == 0){						//オーバーライドおしまい
		motorOutEnable = 1;
		motor->free();
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
