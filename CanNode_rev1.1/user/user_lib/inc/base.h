#ifndef _BASE_H_
#define _BASE_H_

#include "stm32f10x.h"
#include "stdio.h"

//dutyクラス････1~0(もしくは1~-1)で表せる

class dutyOut{
public:
	virtual void write(uint16_t duty){

	}
	virtual void reset(){

	}
	virtual void invert(int8_t mode){

	}
};

class dutyIn{
public:
	virtual int16_t read(){
		return 0;
	}
	virtual void invert(){

	}
};


//countクラス････上限なし
class countIn{
public:
	virtual uint64_t read(){
		return 0;
	}
	virtual void invert(){

	}
};

//Digitalクラス　1か0
class DigitalOut{
public:
	virtual void write(){

	}
	virtual void toggle(){

	}
	virtual void reset(){

	}
	virtual void invert(){

	}
};

class DigitalIn{
public:
	virtual uint16_t read(){
		return 0;
	}
};

#endif
