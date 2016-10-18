#include "SYSTEM.h"

static uint32_t timingDelay;
static uint64_t systemTimer = 0;


void setup(){
	SysTick_Config(SystemCoreClock/1000);//‚ ‚Æ‚Åus‚É‚È‚é‚©‚·
}


extern "C" void SysTick_Handler(void){
	systemTimer++;
	if(timingDelay != 0){
		timingDelay--;
	}
}

void delay(uint32_t nTime){
	timingDelay = nTime;
	while(timingDelay != 0);//À‘•‚ªG‚È‚Ì‚Å1ms‚ÌŒë·‚ğ¶‚¶‚¤‚é
}

uint64_t millis(){
	return systemTimer;
}
