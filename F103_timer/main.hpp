#ifndef MAIN_H_
#define MAIN_H_

#include "system.h"
#include "app.h"
#include "util.h"

#include "CatchRoboMain.h"
#include "CatchRoboMap.h"


System sys;

USART serial,gps;

GPIO segment[11],grid[9];

uint64_t intervalTime;
uint16_t textCnt,commaCnt,hoge = 0;
char outputText[10] = {0,0,0,0,0,0,0,0,0,0},outputTextBuf[10];
/*
uint32_t FlashRead(uint32_t address);
void FlashWrite(uint32_t address,uint32_t data);
bool FlashErase();*/

#define SEG_a 	0
#define SEG_b 	1
#define SEG_c 	2
#define SEG_d 	6
#define SEG_e 	7
#define SEG_f 	9
#define SEG_g 	8
#define SEG_cm	5
#define SEG_dp	4
#define SEG_12	3
#define SEG_da	10

void vfd(uint16_t col,uint16_t num);

void setup(){
	sys.setup();
	serial.setup(USART1_Setup,115200);
	sys.usartSetup(serial);
	delay(100);

	gps.setup(USART2_Setup,57600);

	segment[0].setup(PB11,OUTPUT);
	segment[1].setup(PB10,OUTPUT);
	segment[2].setup(PB2,OUTPUT);
	segment[3].setup(PB1,OUTPUT);
	segment[4].setup(PB0,OUTPUT);
	segment[5].setup(PA7,OUTPUT);
	segment[6].setup(PA2,OUTPUT);
	segment[7].setup(PB5,OUTPUT);
	segment[8].setup(PB7,OUTPUT);
	segment[9].setup(PB8,OUTPUT);
	segment[10].setup(PB9,OUTPUT);

	grid[0].setup(PA4,OUTPUT);
	grid[1].setup(PB6,OUTPUT);
	grid[2].setup(PA5,OUTPUT);
	grid[3].setup(PA8,OUTPUT);
	grid[4].setup(PB15,OUTPUT);
	grid[5].setup(PB14,OUTPUT);
	grid[6].setup(PB13,OUTPUT);
	grid[7].setup(PB12,OUTPUT);
	grid[8].setup(PA6,OUTPUT);
}



#endif
