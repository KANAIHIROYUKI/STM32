#ifndef MAIN_H_
#define MAIN_H_

#include "system.h"
#include "app.h"

TIM pwm[4];

TIM enc[2];
TIM enPwm[2];
CAN can1;
USART serial,vserial;
GPIO add[4],sw[2],led[2],limit[4];
ADC cs[2];

MOTOR motor[2];
CanMotorDriver canMD[2];

#define DEBUG

#define TIM_CNT 1350
//#define TIM_CNT 2048
#define CAN_ADD 15 - (add[0].read()*2) - add[1].read() - (add[2].read()*8) - (add[3].read()*4)

uint16_t rxFlag = 0;
uint8_t canData[8] = {0,0,0,0,0,0,0,0},canAddress=0;
CanRxMsg rxMessage;

#define PRINT_TIME 100

uint64_t printTime = 0;

uint32_t currentInt[2] = {0,0},currentCnt[2] = {0,0};

void setup(){
	systemSetup();

	vserial.setup(USART1,115200,PA10,PA9);

	enc[0].encoderSetup(TIM2,PA0,PA1);
	enc[1].encoderSetup(TIM3,PA6,PA7);

	pwm[0].pwmSetup(TIM4,1,PB6,TIM_CNT);
	pwm[1].pwmSetup(TIM4,2,PB7,TIM_CNT);
	pwm[2].pwmSetup(TIM4,3,PB8,TIM_CNT);
	pwm[3].pwmSetup(TIM4,4,PB9,TIM_CNT);
	enPwm[0].pwmSetup(TIM1,1,PA8,TIM_CNT);
	enPwm[1].pwmSetup(TIM1,2,PA9,TIM_CNT);
	enPwm[0].duty(TIM_CNT);
	enPwm[1].duty(TIM_CNT);

	pwm[0].duty(TIM_CNT);
	pwm[1].duty(TIM_CNT);
	pwm[2].duty(TIM_CNT);
	pwm[3].duty(TIM_CNT);

	add[0].setup(PC13,INPUT_PU);
	add[1].setup(PC14,INPUT_PU);
	add[2].setup(PC15,INPUT_PU);
	add[3].setup(PB2,INPUT_PU);

	sw[0].setup(PB4,INPUT_PU);
	sw[1].setup(PB5,INPUT_PU);
	led[0].setup(PA15,OUTPUT);
	led[1].setup(PB3,OUTPUT);

	led[0].write(Bit_RESET);
	led[1].write(Bit_RESET);

	limit[0].setup(PA2,INPUT_PU);
	limit[1].setup(PA3,INPUT_PU);
	limit[2].setup(PA4,INPUT_PU);
	limit[3].setup(PA5,INPUT_PU);

	motor[0].setup(pwm[0],pwm[1]);
	motor[1].setup(pwm[2],pwm[3]);

	cs[0].setup(ADC1,8,PB0);
	cs[1].setup(ADC2,9,PB1);

	can1.setup(CAN1,PA12,PA11);

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);

	canMD[0].setup(motor[0],can1,CAN_ADD_MOTOR + CAN_ADD);
	canMD[1].setup(motor[1],can1,CAN_ADD_MOTOR + CAN_ADD + 1);

	canMD[0].ledAssign(led[0]);
	canMD[1].ledAssign(led[1]);
}

#endif
