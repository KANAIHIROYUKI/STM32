#include "canNodeMotorDriver.h"

int16_t CanMotorDriver::setup(MOTOR &motor,CAN &can,uint16_t address,uint16_t maxInterval){
	this->canMd_motor = &motor;
	this->canMd_can = &can;
	canMd_address = address;

	canMd_can->filterAdd(canMd_address);

	canMd_receiveTime = 0;
	canMd_maxInterval = maxInterval;

	ledAssigned = 0;
	return 0;
}

void CanMotorDriver::ledAssign(GPIO &led){
	this->canMd_led = &led;
	ledAssigned = 1;
}

void CanMotorDriver::cycle(){
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

void CanMotorDriver::interrupt(CanRxMsg rxMessage){
	if(rxMessage.StdId == canMd_address){
		outDuty = ((rxMessage.Data[1] & 0b01111111) << 8) | rxMessage.Data[0];
		outDuty /= 32768 / (canMd_motor->PWM1->pwm_period >> 1);

		if((rxMessage.Data[1] >> 7) == 1){
			outDuty = -outDuty;
		}

		canMd_motor->duty(outDuty);
		canMd_receiveTime = millis();
	}
}
