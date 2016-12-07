#include "can_valve.hpp"

CanValve::CanValve(Can &can,int canId){
	runSetupOnce=0;
	can.addInterface(this);

	this->canId=canId+0x400;
	pin0.canStatus(0,this);
	pin1.canStatus(1,this);
	pin2.canStatus(2,this);
	pin3.canStatus(3,this);
	pin4.canStatus(4,this);
	pin5.canStatus(5,this);
	pin6.canStatus(6,this);
	pin7.canStatus(7,this);
}

int CanValve::setup(){
	if(runSetupOnce==0){
		if(!canSetup()){
			runSetupOnce++;
			return 0;
		}else
			return 1;
	}
	return 0;
}

void CanValve::canValveTransmit(int arg,unsigned char dataArg[8]){
	canWrite(canId,arg,dataArg);
}

int CanValve::CanValvePin::setup(bool value){
	canValve->setup();

	turnOverValue=value;
	status=1;
	off();

	return 0;
}

int CanValve::CanValvePin::canStatus(int number,CanValve *canValve){
	this->canValve=canValve;
	this->number=number;
	return 0;
}

void CanValve::CanValvePin::on(){
	unsigned char data[8];
	if(turnOverValue==true)
		data[0]=0xfe;
	else
		data[0]=0x01;
	data[0]=data[0]<<(number);
	data[1]=0x01;
	data[1]=data[1]<<(number);

	if(!read())canValve->canValveTransmit(2,data);
	status=1;
}

void CanValve::CanValvePin::off(){
	unsigned char data[8];
	if(turnOverValue==true)
		data[0]=0x01;
	else
		data[0]=0xfe;
	data[0]=data[0]<<(number);
	data[1]=0x01;
	data[1]=data[1]<<(number);

	if(read())canValve->canValveTransmit(2,data);
	status=0;
}

void CanValve::CanValvePin::toggle(){
	if(!read())
		on();
	else
		off();
}
