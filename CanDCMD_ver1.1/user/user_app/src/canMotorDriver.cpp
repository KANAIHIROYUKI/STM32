#include "canMotorDriver.h"

void CanMotorDriver::setup(CAN &can,uint16_t number){
	this->canMotorDriver_can = &can;
	canMotorDriver_address[CAN_MD_ADDRESS_DUTY] = CAN_ADD_DUTY + number;
	canMotorDriver_address[CAN_MD_ADDRESS_FREE] = CAN_ADD_FREE + number;
}

void CanMotorDriver::free(){
	uint8_t canData[2] = {0,0};
	canMotorDriver_can->send(canMotorDriver_address[CAN_MD_ADDRESS_FREE],2,canData);
}

/*
void CanMotorDriver::duty(int16_t motor_duty){
	uint8_t canData[2];
	canData[0] = motor_duty & 0xFF;
	canData[1] = (motor_duty >> 8) & 0xFF;
	canMotorDriver_can->send(canMotorDriver_address,2,canData);

}*/

void CanMotorDriver::duty(float motor_duty){
	outDuty16 = (int16_t)(motor_duty * 32767);

	uint8_t canData[2] = {0,0};
	if(outDuty16 < 0){
		canData[1] = 0x80;
		outDuty16 = -outDuty16;
	}
	canData[0] = outDuty16 & 0xFF;
	canData[1] += (outDuty16 >> 8) & 0x7F;
	canMotorDriver_can->send(canMotorDriver_address[CAN_MD_ADDRESS_DUTY],2,canData);
}


void CanMotorDriver::brake(float motor_brake){
	outDuty16 = (int16_t)(motor_brake * 32767);

	uint8_t canData[2] = {0,0};
	canData[0] = outDuty16 & 0xFF;
	canData[1] += (outDuty16 >> 8) & 0x7F;
	canMotorDriver_can->send(canMotorDriver_address[CAN_MD_ADDRESS_FREE],2,canData);
}
