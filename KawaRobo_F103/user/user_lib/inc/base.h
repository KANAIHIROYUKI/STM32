#ifndef _BASE_H_
#define _BASE_H_

//dutyクラス････1~0(もしくは1~-1)で表せる

class DutyOut{
public:
	void duty(float duty){

	}

	void cycle(){

	}

	void reverse(int16_t mode){

	}
};

class DutyIn{
public:
	float read(){
		return 0;
	}
	void reverse(){

	}
};


//countクラス････上限なし
class CountIn{
public:
	int64_t read(){
		return 0;
	}

	void write(int64_t ){

	}

	void reverse(){

	}
	void reset(){

	}

	void cycle(){

	}
};

//Digitalクラス　1か0
class DigitalOut{
public:
	void write(uint16_t value){

	}
	void toggle(){

	}
	void reset(){

	}
	void invert(uint16_t mode = 1){

	}
};

class DigitalIn{
public:
	uint16_t read(){
		return 0;
	}
};

#endif
