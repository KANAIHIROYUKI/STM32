#include "mcutime.h"
#include "board.h"

int64_t millis(){
	return micros()/1000;
}

void delay_ms(int64_t wait_time){
	int time=0;
	time=millis();
	while(millis()-time<=wait_time);
}

inline void wait(int64_t wait_time){
	delay_ms(wait_time);
}
