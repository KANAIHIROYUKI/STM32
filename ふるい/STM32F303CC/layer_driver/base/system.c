#include "system.h"
#include "mcutime.h"
#include "board.h"
int system_setup(){
	timer_setup();
	board_setup();
	return 0;
}

char* whoami(){
	return board_whoami();
}
