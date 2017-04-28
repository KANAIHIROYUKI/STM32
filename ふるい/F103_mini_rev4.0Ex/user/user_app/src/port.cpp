#include "port.h"


void PORT::write(uint16_t value){
	for(int i=0;i<port_num;i++){
		if(value & (1<<i)){
			io[i].write(Bit_SET);
		}else{
			io[i].write(Bit_SET);
		}
	}
}

uint16_t PORT::read(){
	uint16_t value = 0;
	for(int i=0;i<=port_num;i++){
		value += io[i].read() << i;
	}
	return value;
}

void PORT::invert(){
	for(int i=0;i<=port_num;i++){	//port_num = n�Ȃ̂�<=�ɂ��Ȃ���n+1�񃋁[�v���Ȃ�
		io[i].toggle();
	}
}
