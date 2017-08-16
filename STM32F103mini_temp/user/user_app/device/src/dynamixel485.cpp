#include "dynamixel485.h"

void Dynamixel485::setup(USART &serial,GPIO &dir){
	this->rs485 = &serial;
	this->direction = &dir;
}

void Dynamixel485::cycle(){

}

void Dynamixel485::writeDeg(uint16_t address,int16_t degree){

}

void Dynamixel485::writeDuty(uint16_t address,float duty){

}
