#ifndef MAIN_H_
#define MAIN_H_

#include "system.h"
#include "app.h"
#include "util.h"

System sys;

USART serial;
CAN can;
Switch sw[2];
LED led[6];
ADC analog[4],vBattIn;

TIM pwmP[4],pwmN[4];
GPIO individual,motorEn,spiNss;
NCP5359 motor[4];

SerialArduino sa;
SerialArduino ps3;

CAN_RX rx[4];
CAN_TX tx[4];
Mecanum mc;

#define BUTTON_R2			0
#define BUTTON_L2			1
#define BUTTON_CR			2
#define BUTTON_CI			3
#define HAT_RY				4
#define HAT_RX				5
#define HAT_LY				6
#define HAT_LX				7
#define BUTTON_TR			8
#define BUTTON_SQ			9
#define BUTTON_L1			10
#define BUTTON_R1			11
#define BUTTON_START		12
#define BUTTON_SELECT		13
#define BUTTON_HAT_L		14
#define BUTTON_HAL_R		15

#define SERVO_TILT_LOCK 	0
#define SERVO_HOLDER_LOCK 	1
#define SERVO_GRIP			2
#define SERVO_POUT			3

#define MOTOR_SHAKE			3
#define MOTOR_TILT			2
#define MOTOR_PUSH			0
#define MOTOR_PICKUP		1

#define HAT_TOLERANCE		7

uint64_t intervalTime,intTime;
uint8_t data[8],swStat[16];

int controlMode = 0,ledData = 0;
int servoDeg[8],servoSet[8],servoDef[8];
Average<int> ave;


void servoSwitchEvent(uint8_t ps3Num,uint8_t servoNum){
	if(ps3.read(ps3Num) != swStat[ps3Num]){
		swStat[ps3Num] = ps3.read(ps3Num);

		if(swStat[ps3Num]){
			if(servoDeg[servoNum] != servoDef[servoNum]){
				servoDeg[servoNum] = servoDef[servoNum];
			}else{
				servoDeg[servoNum] = servoSet[servoNum];
			}
			sa.write(servoNum+4,servoDeg[servoNum]);
		}
	}
}

void speak(int voiceTrack,int num){
	voiceTrack += millis()%num;
	serial.printf("*");
	serial.send(voiceTrack);
	return;
}

void speakSwitchEvent(uint8_t ps3Num,uint8_t trackNum,uint8_t rand){
	if(ps3.read(ps3Num) != swStat[ps3Num]){
		swStat[ps3Num] = ps3.read(ps3Num);
		if(swStat[ps3Num]){
			speak(trackNum,rand);
		}
	}
}

int hatTls(int data){
	if(data < HAT_TOLERANCE && data > -HAT_TOLERANCE)data = 0;
	return data;
}




void setup(){
	sys.setup();
	individual.setup(PB2,INPUT_PU);
	motorEn.setup(PA8,OUTPUT);

	pwmP[0].pwmSetup(TIM3,4,PB1,2000);
	pwmP[1].pwmSetup(TIM3,2,PA7,2000);
	pwmP[2].pwmSetup(TIM4,4,PB9,2000);
	pwmP[3].pwmSetup(TIM4,2,PB7,2000);

	pwmN[0].pwmSetup(TIM3,3,PB0,2000);
	pwmN[1].pwmSetup(TIM3,1,PA6,2000);
	pwmN[2].pwmSetup(TIM4,3,PB8,2000);
	pwmN[3].pwmSetup(TIM4,1,PB6,2000);


	for(int i=0;i<4;i++){
		motor[i].setup(pwmP[i],pwmN[i]);
	}

	analog[0].setup(ADC1,0,PA0);
	analog[1].setup(ADC2,1,PA1);
	analog[2].setup(ADC1,4,PA4);
	analog[3].setup(ADC2,5,PA5);

	/*********↓通信↑モーター********/

	ps3.setup(USART1_INIT,115200);
	serial.setup(USART1_INIT,115200);
	serial.printf("DATE = %s\n\r",__DATE__);
	serial.printf("TIME = %s\n\r",__TIME__);

	sys.usartSetup(serial);

	sa.setup(USART3_INIT,115200);

	sw[0].setup(PB5,INPUT_PU);
	sw[1].setup(PB4,INPUT_PU);

	led[0].setup(PC13);
	led[1].setup(PC14);
	led[2].setup(PC15);
	led[3].setup(PA2);
	led[4].setup(PB3);
	led[5].setup(PA15);

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);

	can.setup(CAN1,PA12,PA11);

	if(individual.read()){	//master
		for(int i=0;i<4;i++){
			tx[i].setup(can,0x100 + i);
			//rx[i].setup(can,0x100 + i);
		}
	}else{					//node
		for(int i=0;i<4;i++){
			//tx[i].setup(can,0x100 + i);
			rx[i].setup(can,0x200 + i);
		}
	}
}

extern "C" void USB_LP_CAN1_RX0_IRQHandler(void){
	can.receive();
	for(int i=0;i<4;i++){
		rx[i].interrupt();
	}

	return;
}

#endif
