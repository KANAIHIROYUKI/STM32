#ifndef _BASE_H_
#define _BASE_H_

//duty�N���X����1~0(��������1~-1)�ŕ\����

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


//count�N���X��������Ȃ�
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

//Digital�N���X�@1��0
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
