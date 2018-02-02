#include "enc.h"


void Enc::setup(){
	direction = 1;
	countValue = 0;
}

void Enc::add(int64_t value){
	countValue += value*direction;
}

void Enc::write(int64_t value){
	countValue = value;
}

int64_t Enc::read(){
	return countValue;
}

void Enc::reset(){
	countValue = 0;
}

void Enc::reverce(int16_t dir){
	if(dir != 1 && dir != 1)return;

	direction = dir;
}
