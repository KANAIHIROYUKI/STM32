#ifndef MAIN_H_
#define MAIN_H_

#include "system.h"
#include "app.h"
#include "base.h"

#define CAN_NODE_MODE_VALVE 0
#define CAN_NODE_MODE_AMP   1

#define VALVE_MODE_ADDRESS_ENC 4

System sys;

TIM pwm[4];
TIM enc[4];

CAN can1;
USART serial;
GPIO io[8],led,sw,jumper;

CanNodeValve canValve;
CanNodeEncoder canEnc[2];
CanNodePulse canPulse[4];

//#define DEBUG

uint16_t rxFlag = 0,switchStat = 0,canAddressJumper = 0,printMode = 0;
uint8_t canData[8] = {0,0,0,0,0,0,0,0},canAddress=0,canNodeMode = 0;

#define PRINT_TIME 50

uint64_t printTime = 0,canLastReceiveTime = 0;

void setup(){
	sys.setup();

	serial.setup(USART3,115200,PB10,PB11);
	sys.usartSetup(serial);
	serial.printf("\n\rFILE = %s\n\rDATE = %s\n\rTIME = %s\n\r",__FILE__,__DATE__,__TIME__);

	led.setup(PB2,OUTPUT);
	sw.setup(PC14,INPUT_PU);
	jumper.setup(PC13,INPUT_PU);

	if(jumper.read() == 0){
		canNodeMode = CAN_NODE_MODE_VALVE;
	}else{
		canNodeMode = CAN_NODE_MODE_AMP;
	}

	enc[0].encoderSetup(TIM4,PB6,PB7);
	enc[1].encoderSetup(TIM1,PA8,PA9);

	if(canNodeMode == CAN_NODE_MODE_AMP){
		pwm[0].pwmSetup(TIM2,3,PA2,AMP_PULSE_CYCLE,72,TIM_OCMode_PWM2);
		pwm[1].pwmSetup(TIM2,4,PA3,AMP_PULSE_CYCLE,72,TIM_OCMode_PWM2);
		pwm[2].pwmSetup(TIM3,3,PB0,AMP_PULSE_CYCLE,72,TIM_OCMode_PWM2);
		pwm[3].pwmSetup(TIM3,4,PB1,AMP_PULSE_CYCLE,72,TIM_OCMode_PWM2);

		io[4].setup(PA4,OUTPUT);
		io[5].setup(PA5,OUTPUT);
		io[6].setup(PB12,OUTPUT);
		io[7].setup(PB13,OUTPUT);
	}

	if(canNodeMode == CAN_NODE_MODE_VALVE){
		enc[2].encoderSetup(TIM2,PA0,PA1);
		enc[3].encoderSetup(TIM3,PA6,PA7);

		io[0].setup(PA2,OUTPUT);
		io[1].setup(PA3,OUTPUT);
		io[2].setup(PB0,OUTPUT);
		io[3].setup(PB1,OUTPUT);
		io[4].setup(PA4,OUTPUT);
		io[5].setup(PA5,OUTPUT);
		io[6].setup(PB12,OUTPUT);
		io[7].setup(PB13,OUTPUT);
	}

	can1.setup(CAN1,PA12,PA11);

	if(canNodeMode == CAN_NODE_MODE_AMP){
		canEnc[0].setup(enc[0],can1,4);
		canEnc[1].setup(enc[1],can1,5);

		enc[0].reset();
		enc[1].reset();

		canPulse[0].setup(pwm[0],can1,10);
		canPulse[1].setup(pwm[1],can1,11);
		canPulse[2].setup(pwm[2],can1,12);
		canPulse[3].setup(pwm[3],can1,13);

		canValve.setup(can1,1);
		canValve.pinAdd(io[4]);
		canValve.pinAdd(io[5]);
		canValve.pinAdd(io[6]);
		canValve.pinAdd(io[7]);
	}

	if(canNodeMode == CAN_NODE_MODE_VALVE){
		canEnc[0].setup(enc[0],can1,0);
		canEnc[1].setup(enc[1],can1,1);
		canEnc[2].setup(enc[2],can1,2);
		canEnc[3].setup(enc[3],can1,3);

		enc[0].reset();
		enc[1].reset();
		enc[2].reset();
		enc[3].reset();

		canValve.setup(can1,0);
		canValve.pinAdd(io[0]);
		canValve.pinAdd(io[1]);
		canValve.pinAdd(io[2]);
		canValve.pinAdd(io[3]);
		canValve.pinAdd(io[4]);
		canValve.pinAdd(io[5]);
		canValve.pinAdd(io[6]);
		canValve.pinAdd(io[7]);
	}

	delay(10);
	serial.printf("can rx filter\n\r");
	for(int i=0;i<can1.filterCnt;i++){
		serial.printf("add = 0x%x\n\r",can1.filterAddress[i]);
	}
	delay(10);
}

extern "C" void USB_LP_CAN1_RX0_IRQHandler(void){
	rxFlag++;
	can1.receive();
	if(canNodeMode == CAN_NODE_MODE_AMP){
		canValve.interrupt();

		canEnc[0].interrupt();
		canEnc[1].interrupt();

		canPulse[0].interrupt();
		canPulse[1].interrupt();
		canPulse[2].interrupt();
		canPulse[3].interrupt();
	}


	if(canNodeMode == CAN_NODE_MODE_VALVE){
		canValve.interrupt();

		canEnc[0].interrupt();
		canEnc[1].interrupt();
		canEnc[2].interrupt();
		canEnc[3].interrupt();
	}


	canLastReceiveTime = millis();
	return;
}

#endif
