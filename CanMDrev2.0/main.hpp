#ifndef MAIN_H_
#define MAIN_H_

#include "system.h"
#include "app.h"
#include "base.h"

#define CAN_ADDRESS 15 - (sel[0].read()*8 + sel[1].read() + sel[2].read()*4 + sel[3].read()*2)
#define IntervalTime 50
#define PWM_PERIOD 2000


System sys;

GPIO led[2],sel[4],sw[2],motorEN,virtualEN[2];
TIM enc[2],pwm[4];
USART serial;

Motor motor[2];
CAN can1;

CanNodeMotorDriver canMD[2];

CanMotorDriver canMotor[2];

uint8_t motorDebug[2] = {0,0};
uint64_t intervalTimer = 0;

void setup(){
	sys.setup();

	led[0].setup(PA4,OUTPUT);
	led[1].setup(PA5,OUTPUT);
	sel[0].setup(PB2,INPUT_PU);
	sel[1].setup(PB10,INPUT_PU);
	sel[2].setup(PB1,INPUT_PU);
	sel[3].setup(PB11,INPUT_PU);

	sw[0].setup(PA2,INPUT_PU);
	sw[1].setup(PA3,INPUT_PU);

	motorEN.setup(PB12,OUTPUT);
	motorEN.write(Bit_RESET);



	pwm[0].pwmSetup(TIM4,3,PB8,PWM_PERIOD);
	pwm[1].pwmSetup(TIM4,4,PB9,PWM_PERIOD);
	pwm[2].pwmSetup(TIM4,1,PB6,PWM_PERIOD);
	pwm[3].pwmSetup(TIM4,2,PB7,PWM_PERIOD);

	motor[0].setup(pwm[0],pwm[1]);
	motor[1].setup(pwm[2],pwm[3]);
	motor[0].enPinAssigne(motorEN);
	motor[1].enPinAssigne(motorEN);

	enc[0].encoderSetup(TIM2,PA0,PA1);
	enc[1].encoderSetup(TIM3,PA6,PA7);

	sys.usartSetup(serial);

	can1.setup(CAN1,PA12,PA11);

	canMD[0].setup(motor[0],can1,6);
	canMD[1].setup(motor[1],can1,6 + 1);

	canMotor[0].setup(can1,6);
	canMotor[1].setup(can1,7);

	if(sw[0].read() == 0){
		motorDebug[0] = 1;
		serial.printf("motorA debug\n\r");
		led[0].write(Bit_SET);
	}else{
		canMD[0].ledAssign(led[0]);
	}
	if(sw[1].read() == 0){
		motorDebug[1] = 1;
		serial.printf("motorB debug\n\r");
		led[1].write(Bit_SET);
	}else{
		canMD[1].ledAssign(led[1]);
	}

	serial.setup(USART1,921600,PA9,PA10);
	serial.printf("\n\rFILE = %s\n\r",__FILE__);
	serial.printf("DATE = %s\n\r",__DATE__);
	serial.printf("TIME = %s\n\r",__TIME__);
	serial.printf("ADRS = %d\n\r",CAN_ADDRESS);

	while(sw[0].read() == 0 || sw[1].read() == 0);
	serial.printf("setup end\n\r");
}

extern "C" void USB_LP_CAN1_RX0_IRQHandler(void){
	can1.receive();

	canMD[0].interrupt();
	canMD[1].interrupt();
	return;
}

#endif
