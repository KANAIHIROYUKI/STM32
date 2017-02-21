#ifndef MAIN_H_
#define MAIN_H_

#include "system.h"
#include "app.h"
#include "base.h"

System sys;

TIM pwm[4];
TIM enc[4];

CAN can1;
USART serial;
GPIO io[8],led;

CanNodeValve canValve;
CanNodeEncoder canEnc[4];
CanNodePulse canPulse[4];

CanEncoder canEncoder;
CanValve canVlv;
CanMotorDriver canMD[4];


#define DEBUG



uint16_t rxFlag = 0;
uint8_t canData[8] = {0,0,0,0,0,0,0,0},canAddress=0;
CanRxMsg rxMessage;

#define PRINT_TIME 50

uint64_t printTime = 0;

uint32_t currentInt[2] = {0,0},currentCnt[2] = {0,0};

void setup(){
	sys.setup();

	serial.setup(USART3,921600,PB10,PB11);
	sys.usartSetup(serial);
	serial.printf("\n\rFILE = %s\n\rDATE = %s\n\rTIME = %s\n\r",__FILE__,__DATE__,__TIME__);


	enc[0].encoderSetup(TIM1,PA8,PA9);
	enc[1].encoderSetup(TIM2,PA0,PA1);
	enc[2].encoderSetup(TIM3,PA6,PA7);
	enc[3].encoderSetup(TIM4,PB6,PB7);

	led.setup(PB2,OUTPUT);

	io[0].setup(PA2,OUTPUT);
	io[1].setup(PA3,OUTPUT);
	io[2].setup(PB0,OUTPUT);
	io[3].setup(PB1,OUTPUT);
	io[4].setup(PA4,OUTPUT);
	io[5].setup(PA5,OUTPUT);
	io[6].setup(PB12,OUTPUT);
	io[7].setup(PB13,OUTPUT);

	can1.setup(CAN1,PA12,PA11);

	canEnc[0].setup(enc[0],can1,0);
	canEnc[1].setup(enc[1],can1,1);
	canEnc[2].setup(enc[2],can1,2);
	canEnc[3].setup(enc[3],can1,3);

	canPulse[0].setup(io[0],can1,0);
	canPulse[1].setup(io[1],can1,1);
	canPulse[2].setup(io[2],can1,2);
	canPulse[3].setup(io[3],can1,3);

	canValve.setup(can1,0);
	//canValve.pinAdd(io[0]);
	//canValve.pinAdd(io[1]);
	//canValve.pinAdd(io[2]);
	//canValve.pinAdd(io[3]);
	canValve.pinAdd(io[4]);
	canValve.pinAdd(io[5]);
	canValve.pinAdd(io[6]);
	canValve.pinAdd(io[7]);

	//canEncoder.setup(can1,0,10);
	//canVlv.setup(can1,0);
	//canMD[0].setup(can1,0);
	//canMD[1].setup(can1,1);
	//canMD[2].setup(can1,2);
	//canMD[3].setup(can1,3);
}

extern "C" void USB_LP_CAN1_RX0_IRQHandler(void){
	rxFlag++;
	can1.receive();
	canValve.interrupt();

	canEnc[0].interrupt();
	canEnc[1].interrupt();
	canEnc[2].interrupt();
	canEnc[3].interrupt();

	//canEncoder.interrupt();

	canPulse[0].interrupt();
	canPulse[1].interrupt();
	canPulse[2].interrupt();
	canPulse[3].interrupt();
	return;
}

#endif
