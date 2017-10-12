#ifndef MAIN_H_
#define MAIN_H_

#include "system.h"
#include "app.h"
#include "util.h"

#define IntervalTime 10

#define BAUDRATE 57600　// ディフォルトボーレート[bps]
#define ADDRESS_GOAL_POSITION 30 // コントロールテーブルでの目標座標のアドレス
#define RX28_HAND 0 // サーボAのモータID(0〜253)
#define RX28_BASE 1 // サーボBのモータID(0〜253)
#define RS485RTS 2 // RS485の送受切替信号。RTSをHIGH固定接続なら不要

System sys;

USART serial,rs485;
GPIO rs485_dir,io[10];
Switch sw[2];
LED led[4];

DXLIB sr518;

uint64_t intervalTime;

void servo(int mID, int mPos){
	  int sum;
	  unsigned char buf[10];

	  buf[0] = 0xFF;
	  buf[1] = 0xFF;
	  buf[2] = (unsigned char)mID;
	  sum = mID;
	  buf[3] = (unsigned char)(3 + 2);
	  sum += buf[3];
	  buf[4] = 0x03;
	  sum += buf[4];
	  buf[5] = (unsigned char)ADDRESS_GOAL_POSITION;
	  sum += buf[5];
	  buf[6] = (unsigned char)(mPos & 0xFF);
	  sum += buf[6];
	  buf[7] = (unsigned char)(mPos >> 8);
	  sum += buf[7];
	  buf[8] = (unsigned char)(~(sum) & 0xFF);

	  rs485_dir.write(1);
	  delayMicros(10);
	  for(int i = 0; i < 9 ; i++){
	    rs485.send(buf[i]);
	  }
	  rs485_dir.write(0);
}

void setup(){
	sys.setup();

	/*********↓通信↑モーター********/

	serial.setup(USART1,115200,PA9,PA10);
	serial.printf("DATE = %s\n\r",__DATE__);
	serial.printf("TIME = %s\n\r",__TIME__);

	sys.usartSetup(serial);

	rs485.setup(USART2,500000,PA2,PA3);
	rs485_dir.setup(PB1,OUTPUT);
	sr518.setup(rs485,rs485_dir);

	sw[0].setup(PB0,INPUT_PU);
	sw[1].setup(PA7,INPUT_PU);

	led[0].setup(PC13);
	led[1].setup(PC14);
	led[2].setup(PC15);
	led[3].setup(PB2);

	io[0].setup(PB5,OUTPUT);
	io[1].setup(PB4,OUTPUT);
	io[2].setup(PB3,OUTPUT);
	io[3].setup(PA15,OUTPUT);

	io[0].setup(PB9,OUTPUT);
	io[1].setup(PB8,OUTPUT);
	io[2].setup(PB7,OUTPUT);
	io[3].setup(PB6,OUTPUT);

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);

	intervalTime = 0;

}

#endif
