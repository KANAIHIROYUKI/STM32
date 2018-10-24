#ifndef MAIN_H_
#define MAIN_H_

#include "system.h"
#include "app.h"
#include "util.h"

#define MotorR_S 1
#define MotorS_S 0
#define MotorR 3
#define MotorS 2
#define ServoZ 4
#define ServoP 5

#define MoveX	6
#define MoveY	7
#define MoveR	8

System sys;

USART serial;
SBDBT ps3;

CAN can;

LED led[3];

ADC pot;
TIM enc;
Switch limit;

CanMotorDriver md[4],servo[2];
CanValve valve;

CAN_TX tx[4];

uint64_t intervalTime,canIntervalTime,controlCycleTime;
float motorDuty[10];
int handStat = 0,oldHand = 0;
uint8_t vData[8];

void controlCycle();

void setup(){
	sys.setup();
	serial.setup(USART1_Setup,115200);
	sys.usartSetup(serial);

	//ポテンショ
	pot.setup(ADC14_Setup);
	//pot[1].setup(ADC15_Setup);
	//pot[2].setup(ADC16_Setup);

	//エンコーダ
	enc.encoderSetup(TIM2_ENC_Setup);
	//enc[1].encoderSetup(TIM4_ENC_Setup);

	//リミット
	limit.setup(PA7);
	//limit[1].setup(PB1);

	//PS3コン
	ps3.setup(USART3_Setup,115200);
	can.setup(CAN1_Setup);

	//モーター
	md[0].setup(can,0);
	md[1].setup(can,1);
	md[2].setup(can,2);
	md[3].setup(can,3);

	//サーボ
	servo[0].setup(can,ServoZ);
	servo[1].setup(can,ServoP);

	//電磁弁
	valve.setup(can,0);

	//LED
	led[0].setup(PC13);
	led[1].setup(PC14);
	led[2].setup(PC15);

	led[0].interval(500);


	for(int i=0;i<4;i++){
		tx[i].setup(can,0x200 + i);
	}
}



#endif
