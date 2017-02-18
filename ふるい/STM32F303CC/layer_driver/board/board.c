#include "board.h"


//muctime.h begin *************************
#include "config_systick.h"

static int64_t g_timer=0;

void SysTick_Handler(void){
	//任意ms毎に割り込んで、1カウントする　単位:[ms]
	g_timer++;
}

void timer_setup(){
	Init_SysTick(0.00001);
}


int64_t micros(){
	return g_timer*10;
}
//muctime.h end ***************************

//software_reset begin ********************
#include "iwdg_software_reset.h"

void stm32_software_reset(){
	soft_reset_durch();
}
//software_reset end **********************

//watchdog begin **************************
#include "config_iwdg.h"
int stm32_watchdog_setup(){
	return Init_watchdog();
}

void stm32_watchdog_cycle(){
	watchdog_reset();
}
//watchdog end ****************************

//system.h begin **************************
#include "system_stm32f4xx.h"
#include "pin.hpp"
#include <stdio.h>
#include "serial.hpp"
int board_setup(){
	SystemInit();
	setvbuf( stdout, 0, _IONBF, 0 );
	std_char_out_setup();
	timer_setup();
	return 0;
}

char* board_whoami(){
	return board_name();
}
//system.h end ****************************
