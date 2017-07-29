#ifndef _BASE_H_
#define _BASE_H_

#include "stm32f10x.h"
#include "stdio.h"

//dutyクラス････1~0(もしくは1~-1)で表せる

class DutyOut{
public:
	virtual void duty(float duty){

	}
	virtual void reset(){

	}
	virtual void invert(int16_t mode){

	}
};

class DutyIn{
public:
	virtual float read(){
		return 0;
	}
	virtual void invert(){

	}
};


//countクラス････上限なし
class CountIn{
public:
	virtual int64_t read(){
		return 0;
	}
	virtual void invert(){

	}
	virtual void reset(){

	}
};

//Digitalクラス　1か0
class DigitalOut{
public:
	virtual void write(uint16_t value){

	}
	virtual void toggle(){

	}
	virtual void reset(){

	}
	virtual void invert(uint16_t mode = 1){

	}
};

class DigitalIn{
public:
	virtual uint16_t read(){
		return 0;
	}
};

#endif
