#ifndef MAIN_H_
#define MAIN_H_

#include "system.h"
#include "app.h"

TIM pwm[4];
TIM enc[4];

CAN can1;
USART serial;
GPIO io[8],led;

CanNodeValve canValve;
CanNodeEncoder canEnc[4];

CanEncoder canEncoder;

#define DEBUG



uint16_t rxFlag = 0;
uint8_t canData[8] = {0,0,0,0,0,0,0,0},canAddress=0;
CanRxMsg rxMessage;

#define PRINT_TIME 100

uint64_t printTime = 0;

uint32_t currentInt[2] = {0,0},currentCnt[2] = {0,0};

void setup(){
	systemSetup();

	serial.setup(USART3,921600,PB10,PB11);

	enc[0].encoderSetup(TIM1,PA8,PA9);
	enc[1].encoderSetup(TIM4,PB6,PB7);

	pwm[0].pwmSetup(TIM2,1,PA2,10000,72);
	pwm[1].pwmSetup(TIM2,2,PA3,10000,72);
	pwm[2].pwmSetup(TIM3,3,PB0,10000,72);
	pwm[3].pwmSetup(TIM3,4,PB1,10000,72);

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

	canValve.setup(can1,0);
	canValve.pinAdd(io[0]);
	canValve.pinAdd(io[1]);
	canValve.pinAdd(io[2]);
	canValve.pinAdd(io[3]);
	canValve.pinAdd(io[4]);
	canValve.pinAdd(io[5]);
	canValve.pinAdd(io[6]);
	canValve.pinAdd(io[7]);

	led.write(Bit_RESET);

	canEncoder.setup(can1,0,10);


	serial.printf("\n\rFILE = %s\n\rDATE = %s\n\rTIME = %s\n\r",__FILE__,__DATE__,__TIME__);
}

extern "C" void USB_LP_CAN1_RX0_IRQHandler(void){
	rxFlag++;
	can1.receive();
	canValve.interrupt();
	canEnc[0].interrupt();
	canEncoder.interrupt();
	return;
}

#endif
