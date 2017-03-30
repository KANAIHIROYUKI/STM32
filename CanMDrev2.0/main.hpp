#ifndef MAIN_H_
#define MAIN_H_

#include "system.h"
#include "app.h"
#include "base.h"

#define CAN_ADDRESS 15 - (sel[0].read()*8 + sel[1].read() + sel[2].read()*4 + sel[3].read()*2)
#define IntervalTime 50
#define PWM_PERIOD 2000

System sys;

GPIO led[2],sel[4],swPin[2],motorEN,virtualEN[2],limitPin[4];
TIM enc[2],pwm[4];
USART serial;

Switch limit[4],sw[2];
Motor motor[2];
CAN can1;

CanNodeMotorDriver canMD[2];
CanNodeEncoder canEnc[2];
CanNodeSwitch canSw[4];

CanMotorDriver canMotor[2];
CanEncoder canEncoder[2];
CanSwitch canSwitch[4];


uint8_t motorDebug[2] = {0,0},canRTR=0,canData[8],debugMode;
uint64_t intervalTimer = 0;

void setup(){
	sys.setup();

	led[0].setup(PA4,OUTPUT);
	led[1].setup(PA5,OUTPUT);
	sel[0].setup(PB2,INPUT_PU);
	sel[1].setup(PB10,INPUT_PU);
	sel[2].setup(PB1,INPUT_PU);
	sel[3].setup(PB11,INPUT_PU);

	swPin[0].setup(PA2,INPUT_PU);
	swPin[1].setup(PA3,INPUT_PU);
	sw[0].setup(swPin[0]);
	sw[1].setup(swPin[1]);

	limitPin[0].setup(PB13,INPUT_PU);
	limitPin[1].setup(PB14,INPUT_PU);
	limitPin[2].setup(PB15,INPUT_PU);
	limitPin[3].setup(PA8,INPUT_PU);

	limit[0].setup(limitPin[0]);
	limit[1].setup(limitPin[1]);
	limit[2].setup(limitPin[2]);
	limit[3].setup(limitPin[3]);

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
	if(swPin[0].read() == 0 || swPin[1].read() == 0 ){
		debugMode = 1;
		can1.debug();
	}


	canMD[0].setup(motor[0],can1,(CAN_ADDRESS * 2));
	canMD[1].setup(motor[1],can1,(CAN_ADDRESS * 2) + 1);

	canEnc[0].setup(enc[0],can1,(CAN_ADDRESS * 2));
	canEnc[1].setup(enc[1],can1,(CAN_ADDRESS * 2) + 1);

	canSw[0].setup(limit[0],can1,(CAN_ADDRESS * 2));
	canSw[1].setup(limit[2],can1,(CAN_ADDRESS * 2) + 1);
	//canSw[2].setup(limit[2],can1,(CAN_ADDRESS * 2) + 2);
	//canSw[3].setup(limit[3],can1,(CAN_ADDRESS * 2) + 3);

	if(debugMode){
		canEncoder[0].setup(can1,(CAN_ADDRESS * 2),10);
		canEncoder[1].setup(can1,(CAN_ADDRESS * 2) + 1,10);

		canMotor[0].setup(can1,(CAN_ADDRESS * 2));
		canMotor[1].setup(can1,(CAN_ADDRESS * 2) + 1);

		canSwitch[0].setup(can1,(CAN_ADDRESS * 2),100);
		canSwitch[1].setup(can1,(CAN_ADDRESS * 2) + 1,100);
		//canSwitch[2].setup(can1,(CAN_ADDRESS * 2) + 2,100);
		//canSwitch[3].setup(can1,(CAN_ADDRESS * 2) + 3,100);
	}


	if(swPin[0].read() == 0){
		motorDebug[0] = 1;
		serial.printf("motorA debug\n\r");
		led[0].write(Bit_SET);
	}else{
		canMD[0].ledAssign(led[0]);
	}
	if(swPin[1].read() == 0){
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

	while(swPin[0].read() == 0 && swPin[1].read() == 0);
	serial.printf("setup end\n\r");
}

extern "C" void USB_LP_CAN1_RX0_IRQHandler(void){
	can1.receive();
	canMD[0].interrupt();
	canMD[1].interrupt();

	canEnc[0].interrupt();
	canEnc[1].interrupt();

	canSw[0].interrupt();
	canSw[1].interrupt();
	//canSw[2].interrupt();
	//canSw[3].interrupt();

	if(debugMode){
		canEncoder[1].interrupt();
		canEncoder[0].interrupt();

		canSwitch[0].interrupt();
		canSwitch[1].interrupt();
		//canSwitch[2].interrupt();
		//canSwitch[3].interrupt();
	}


	return;
}

#endif
