#ifndef MAIN_H_
#define MAIN_H_

#include "system.h"
#include "app.h"
#include "base.h"

System sys;

TIM pwm[3];
TIM trigger;
GPIO en[3],zeroCross[3],led[2],sw[2],sell[4],hall[3];
ADC cs;

CAN can1;
USART serial;

Phase phase[3];
BLDC bldc;

#define DEBUG

#define ADVANCED_ANGLE (float)0.95
#define CONTROL_ADJUST_TIME 10

#define ADDRESS (15 - (sell[1].read() + (sell[0].read()*2) + (sell[3].read()*4) + (sell[2].read()*8)))

int16_t hallToDegree[] = {-1,30,270,330,150,90,210,-1};
uint16_t rxFlag = 0;

uint64_t hallInterruptTime,hallIntervalTime;

#define PRINT_TIME 10
uint64_t printTime = 0,interruptTime;

uint16_t adcCnt = 0,adcValue,phaseStat = 0,interruptHallNumber;

int16_t hallDegree = 0,hallNum,outDegree,triggerStat = 0,outTriggerDegree[2];
uint16_t driveStat = 0,driveEnable = 0,driveMode = 0,triggerCnt[2] = {0,0};

uint64_t hallRaiseTime[3],hallFallTime[3],hallLowTime[3],hallHighTime[3];
uint64_t speedLimitTime;

float outPower;

void hallInterruptFunc();

void setup(){
	sys.setup();

	serial.setup(USART1,921600,PA9,PA10);
	sys.usartSetup(serial);
	serial.printf("\n\rFILE = %s\n\rDATE = %s\n\rTIME = %s\n\r",__FILE__,__DATE__,__TIME__);

	trigger.timerSetup(TIM2);
	trigger.itSetup(TIM_IT_CC1);
	trigger.stop();

	pwm[1].pwmSetup(TIM3,1,PA6,1000);
	pwm[0].pwmSetup(TIM3,2,PA7,1000);
	pwm[2].pwmSetup(TIM3,3,PB0,1000);

	en[1].setup(PA3,OUTPUT);
	en[0].setup(PA4,OUTPUT);
	en[2].setup(PA5,OUTPUT);

	phase[0].setup(pwm[0],en[0]);
	phase[1].setup(pwm[1],en[1]);
	phase[2].setup(pwm[2],en[2]);

	cs.setup(ADC1,2,PA2);

	bldc.setup(phase[0],phase[1],phase[2]);

	led[0].setup(PB5,OUTPUT);
	led[1].setup(PB6,OUTPUT);

	sw[0].setup(PB7,INPUT_PU);
	sw[1].setup(PB8,INPUT_PU);

	hall[0].setup(PA15,INPUT_PU);
	hall[1].setup(PB3,INPUT_PU);
	hall[2].setup(PB4,INPUT_PU);

	zeroCross[0].setup(PB12,INPUT_PU);
	zeroCross[1].setup(PB13,INPUT_PU);
	zeroCross[2].setup(PB14,INPUT_PU);

	hall[0].interruptSetup(EXTI_Trigger_Rising_Falling);
	hall[1].interruptSetup(EXTI_Trigger_Rising_Falling);
	hall[2].interruptSetup(EXTI_Trigger_Rising_Falling);

	sell[0].setup(PC15,INPUT_PU);
	sell[1].setup(PC14,INPUT_PU);
	sell[2].setup(PB9,INPUT_PU);
	sell[3].setup(PC13,INPUT_PU);

	can1.setup(CAN1,PA12,PA11);

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);

	serial.printf("end %d\n\r",ADDRESS);
}

extern "C" void USB_LP_CAN1_RX0_IRQHandler(void){
	rxFlag++;
	can1.receive();
	return;
}

extern "C" void ADC1_2_IRQHandler(void){
	adcCnt++;
	cs.interrupt();

	adcValue = cs.peek();
	cs.start(ADC_SampleTime_239Cycles5);
}

extern "C" void EXTI15_10_IRQHandler(void){
	hall[0].interruptFrag();	//フラグリセット
	interruptHallNumber = 0;
	hallInterruptFunc();
}

extern "C" void  EXTI3_IRQHandler(void){
	hall[1].interruptFrag();	//フラグリセット
	interruptHallNumber = 1;
	hallInterruptFunc();
}

extern "C"void EXTI4_IRQHandler(void){
	hall[2].interruptFrag();
	interruptHallNumber = 2;
	hallInterruptFunc();
}

#endif
