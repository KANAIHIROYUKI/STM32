#include "canMotorDriver.h"

void CanMotorDriver::setup(CAN &can,uint16_t number){
	this->canMotorDriver_can = &can;
	canMotorDriver_address = CAN_ADD_DUTY + number;
}

void CanMotorDriver::duty(int16_t motor_duty){
	uint8_t canData[2];
	canData[0] = motor_duty;
	canData[1] = motor_duty >> 8;
	canMotorDriver_can->send(canMotorDriver_address,2,canData);

}
