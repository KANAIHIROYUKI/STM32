#include "stm32f10x.h"
#include "stdio.h"
#include "system.h"
#include "app.h"

#define IntervalTime 100

#define CP_ver10 0
#define CP_ver11 1

#define CAN_ADD_SWITCH_SETUP 0x700
#define CAN_ADD_SWITCH_VALUE 0x740

#define ROTARY 15 - ((switchPin[3].read() << 3) + (switchPin[0].read() << 2) + (switchPin[2].read() << 1) + (switchPin[1].read() << 0))

uint64_t intervalTime = 0,transmitTime = 0,transmitIntervalTime = 0;
uint16_t rxFlag,rtrFlag;


GPIO signal,led[16],switchPin[8];
USART serial;
CAN can1;

CanNodeValve canVlv[2];

CanValve canValve[2];
CanEncoder canSwitch;

void setup(){

	uint8_t revision = CP_ver11;

	systemSetup();

	signal.setup(PB0,OUTPUT);

	led[0].setup(PB9,OUTPUT);
	led[1].setup(PB8,OUTPUT);
	led[2].setup(PB7,OUTPUT);
	led[3].setup(PB6,OUTPUT);
	led[4].setup(PB5,OUTPUT);
	led[5].setup(PB12,OUTPUT);
	led[6].setup(PB13,OUTPUT);
	led[7].setup(PB14,OUTPUT);

	led[8].setup(PB15,OUTPUT);
	led[9].setup(PA5,OUTPUT);
	led[10].setup(PA6,OUTPUT);
	led[11].setup(PA7,OUTPUT);
	led[12].setup(PB1,OUTPUT);
	led[13].setup(PB2,OUTPUT);
	led[14].setup(PB10,OUTPUT);
	led[15].setup(PB11,OUTPUT);

	switchPin[0].setup(PC13,INPUT_PU);
	switchPin[1].setup(PC14,INPUT_PU);
	switchPin[2].setup(PC15,INPUT_PU);
	switchPin[3].setup(PA0,INPUT_PU);

	switchPin[4].setup(PA1,INPUT_PU);
	switchPin[5].setup(PA2,INPUT_PU);
	switchPin[6].setup(PA3,INPUT_PU);
	switchPin[7].setup(PA4,INPUT_PU);

	can1.setup(CAN1,PA12,PA11);
	can1.filterAdd(CAN_ADD_SWITCH_VALUE,CAN_ADD_SWITCH_SETUP);

	canVlv[0].setup(can1,0x10);
	canVlv[1].setup(can1,0x11);

	for(int i=0;i<8;i++){
		canVlv[0].pinAdd(led[i]);
		canVlv[1].pinAdd(led[i + 8]);
	}

	for(int i=0;i<16;i++){
		led[i].write(1);
		delay(7);
	}
	for(int i=0;i<16;i++){
		led[i].write(0);
		delay(7);
	}

	//canValve[0].setup(can1,0x10);
	//canValve[1].setup(can1,0x11);

	//canSwitch.setup(can1,CAN_ADD_SWITCH_SETUP - CAN_ADD_ENC_SETUP,1000);

	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);

	serial.setup(USART1,921600,PA9,PA10);
	serial.printf("\n\rFILE = %s\n\r",__FILE__);
	serial.printf("DATE = %s\n\r",__DATE__);
	serial.printf("TIME = %s\n\r",__TIME__);
	if(revision == CP_ver10){
		serial.printf("ControlPanel hrev 1.0\n\r");
	}else if(revision == CP_ver11){
		serial.printf("ControlPanel rev 1.1\n\r");
	}
}




extern "C" void USB_LP_CAN1_RX0_IRQHandler(void){
	rxFlag++;
	can1.receive();
	canVlv[0].interrupt();
	canVlv[1].interrupt();
	//canSwitch.interrupt();
	if(can1.rxMessage.StdId == CAN_ADD_SWITCH_SETUP){
		if(can1.rxMessage.Data[0] == 1){
			transmitIntervalTime = (can1.rxMessage.Data[2] << 8) + can1.rxMessage.Data[1];
		}else{

		}
	}
	return;
}

