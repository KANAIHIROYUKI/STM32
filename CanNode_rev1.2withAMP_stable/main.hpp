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
GPIO io[8],led,sw,jumper;

CanNodeValve canValve;
CanNodeEncoder canEnc[2];
CanNodePulse canPulse[4];

CanEncoder canEncoder;
CanValve canVlv;
CanMotorDriver canMD[4];

//#define DEBUG

uint16_t rxFlag = 0,switchStat = 0,canAddressJumper = 0,printMode = 0;
uint8_t canData[8] = {0,0,0,0,0,0,0,0},canAddress=0;

#define PRINT_TIME 50

uint64_t printTime = 0,canLastReceiveTime = 0;

void setup(){
	sys.setup();

	serial.setup(USART3,921600,PB10,PB11);
	sys.usartSetup(serial);
	serial.printf("\n\rFILE = %s\n\rDATE = %s\n\rTIME = %s\n\r",__FILE__,__DATE__,__TIME__);

	enc[0].encoderSetup(TIM1,PA8,PA9);
	enc[1].encoderSetup(TIM4,PB6,PB7);

	enc[0].reverse();


	pwm[0].pwmSetup(TIM2,3,PA2,AMP_PULSE_CYCLE,72,TIM_OCMode_PWM2);
	pwm[1].pwmSetup(TIM2,4,PA3,AMP_PULSE_CYCLE,72,TIM_OCMode_PWM2);
	pwm[2].pwmSetup(TIM3,3,PB0,AMP_PULSE_CYCLE,72,TIM_OCMode_PWM2);
	pwm[3].pwmSetup(TIM3,4,PB1,AMP_PULSE_CYCLE,72,TIM_OCMode_PWM2);

	led.setup(PB2,OUTPUT);
	sw.setup(PC14,INPUT_PU);
	jumper.setup(PC13,INPUT_PU);

	io[4].setup(PA4,OUTPUT);
	io[5].setup(PA5,OUTPUT);
	io[6].setup(PB12,OUTPUT);
	io[7].setup(PB13,OUTPUT);

	can1.setup(CAN1,PA12,PA11);

	canEnc[0].setup(enc[0],can1,4);
	canEnc[1].setup(enc[1],can1,5);

	canPulse[0].setup(pwm[0],can1,10);
	canPulse[1].setup(pwm[1],can1,11);
	canPulse[2].setup(pwm[2],can1,12);
	canPulse[3].setup(pwm[3],can1,13);

	canValve.setup(can1,0);
	canValve.pinAdd(io[4]);
	canValve.pinAdd(io[5]);
	canValve.pinAdd(io[6]);
	canValve.pinAdd(io[7]);

#ifdef DEBUG
	canEncoder.setup(can1,4,10);
	canVlv.setup(can1,0);
#endif

	canMD[0].setup(can1,10);
	canMD[1].setup(can1,11);
	canMD[2].setup(can1,12);
	canMD[3].setup(can1,13);

	enc[0].reset();
	enc[1].reset();

	serial.printf("enc setup address = %x,%x\n\r",canEnc[0].canEnc_address,canEnc[1].canEnc_address);
	serial.printf("enc valur address = %x,%x\n\r",canEnc[0].canEnc_address + 0x40,canEnc[1].canEnc_address + 0x40);
	serial.printf("vlv address =  %x,filter number = %d\n\r",canValve.canValve_address,can1.filterCnt);
	serial.printf("pls address = %x,%x,%x,%x\n\r",canPulse[0].canPulse_address,canPulse[1].canPulse_address,canPulse[2].canPulse_address,canPulse[3].canPulse_address);

	delay(10);

	for(int i=0;i<can1.filterCnt;i++){
		serial.printf("filter number = %d,address = 0x%x\n\r",i,can1.filterAddress[i]);
	}
}

extern "C" void USB_LP_CAN1_RX0_IRQHandler(void){
	rxFlag++;
	can1.receive();
	canValve.interrupt();

	canEnc[0].interrupt();
	canEnc[1].interrupt();

#ifdef DEBUG
	canEncoder.interrupt();
#endif

	canPulse[0].interrupt();
	canPulse[1].interrupt();
	canPulse[2].interrupt();
	canPulse[3].interrupt();

	canLastReceiveTime = millis();
	return;
}

#endif
