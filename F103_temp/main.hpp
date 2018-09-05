#ifndef MAIN_H_
#define MAIN_H_

#include "system.h"
#include "app.h"
#include "util.h"

#include "CatchRoboMain.h"
#include "CatchRoboMap.h"


System sys;

USART serial;
SBDBT ps3;
GPIO id;

CAN can;

LED led[3];

ADC pot[3];
TIM enc[2];
Switch limit[2];

CanMotorDriver md[4],servo[2];
CanValve valve;

uint64_t intervalTime,canIntervalTime;
/*
uint32_t FlashRead(uint32_t address);
void FlashWrite(uint32_t address,uint32_t data);
bool FlashErase();*/

CatchRobo robo;
CatchRoboMap map;

float motorDuty[4];
uint64_t rxTime;

uint8_t canData[8];

void setup(){
	sys.setup();
	serial.setup(USART1_Setup,115200);
	sys.usartSetup(serial);

	//ポテンショ
	pot[0].setup(ADC14_Setup);
	pot[1].setup(ADC15_Setup);
	pot[2].setup(ADC16_Setup);

	robo.assignAD(pot[0],0);
	robo.assignAD(pot[1],1);
	robo.assignAD(pot[2],2);

	//エンコーダ
	enc[0].encoderSetup(TIM2_ENC_Setup);
	enc[1].encoderSetup(TIM4_ENC_Setup);

	robo.assignENC(enc[0],0);
	robo.assignENC(enc[1],1);

	id.setup(PA6,INPUT_PU);

	//リミット
	limit[0].setup(PA7);
	limit[1].setup(PB1);

	robo.assignLimit(limit[0],0);
	robo.assignLimit(limit[1],1);

	//PS3コン
	ps3.setup(USART3_Setup,115200);

/*	while(1){
		limit[0].cycle();
		limit[1].cycle();
		if(intervalTime < millis()){
			intervalTime += 100;
			serial.printf("%d,%d\n\r",limit[0].read(),limit[1].read());
		}
	}*/

	can.setup(CAN1_Setup);

	//モーター
	md[0].setup(can,0);
	md[1].setup(can,1);
	md[2].setup(can,2);
	md[3].setup(can,3);

	robo.assignMD(md[0],0);
	robo.assignMD(md[1],1);
	robo.assignMD(md[2],2);
	robo.assignMD(md[3],3);

	//サーボ
	servo[0].setup(can,0x4);
	servo[1].setup(can,0x5);

	robo.assignMD(md[4],4);
	robo.assignMD(md[5],5);

	//電磁弁
	valve.setup(can,0);

	robo.assignValve(valve);

	//LED
	led[0].setup(PC13);
	led[1].setup(PC14);
	led[2].setup(PC15);

	led[0].interval(500);

	/*while(1){
		for(int i=0;i<10;i++){
			servo[0].position(-90);
			servo[1].position(-90);
			delay(50);

		}

		for(int i=0;i<10;i++){
			servo[0].position(180);
			servo[1].position(180);
			delay(50);
		}
	}*/

	robo.setup(serial,ps3,id.read());

}



#endif
