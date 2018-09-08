#include "canMotorDriver.h"

void CanMotorDriver::setup(CAN &can,uint16_t number){
	this->can = &can;
	address = number;

}

void CanMotorDriver::free(){
	uint8_t canData[2] = {0,0};
	can->send(address + CAN_ADD_FREE,2,canData);
}

void CanMotorDriver::duty(float motor_duty){
	if(motor_duty < -1)motor_duty = -1;
	if(motor_duty > 1)motor_duty = 1;
	outDuty16 = (int16_t)(motor_duty * 32767);

	uint8_t data[2] = {0,0};
	if(outDuty16 < 0){
		data[1] = 0x80;
		outDuty16 = -outDuty16;
	}
	data[0] = outDuty16 & 0xFF;
	data[1] += (outDuty16 >> 8) & 0x7F;
	can->send(address + CAN_ADD_DUTY,2,data);
}

void CanMotorDriver::position(float pos){
	uint8_t data[4];
	float_to_uchar4(data,pos);
	can->send(address + CAN_ADD_POSITION,8,data);
	outPosition = pos;
}

void CanMotorDriver::brake(float motor_brake){
	outDuty16 = (int16_t)(motor_brake * 32767);

	uint8_t data[2] = {0,0};
	data[0] = outDuty16 & 0xFF;
	data[1] += (outDuty16 >> 8) & 0x7F;
	can->send(address + CAN_ADD_FREE,2,data);
}
