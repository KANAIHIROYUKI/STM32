#ifndef MAIN_H_
#define MAIN_H_

#include "system.h"
#include "app.h"
#include "base.h"

#define CAN_ADDRESS (uint16_t)(15 - (sel[0].read()*8 + sel[1].read() + sel[2].read()*4 + sel[3].read()*2))
#define IntervalTime 50
#define PWM_PERIOD 2000

System sys;

GPIO led[2],sel[4];
TIM enc[2],pwm[4],en[2];
USART serial,iso;

Switch limit[4],sw[2];
Motor motor[2];
CAN can1;

SI8900 isoIn;

CanNodeMotorDriver canMD[2];
CanNodeEncoder canEnc[2];
CanNodeSwitch canSw;
CanNodeVoltage canVol;

CanMotorDriver canMotor[2];
CanEncoder canEncoder[2];
CanSwitch canSwitch;
CanVoltage canVoltage;


uint8_t motorDebug[2] = {0,0},canRTR=0,canData[8],debugMode,printNum;
uint64_t intervalTimer = 0;

void setup(){
	sys.setup();

	led[0].setup(PA15,OUTPUT);
	led[1].setup(PB3,OUTPUT);

	sel[0].setup(PB15,INPUT_PU);
	sel[1].setup(PB14,INPUT_PU);
	sel[2].setup(PB13,INPUT_PU);
	sel[3].setup(PB12,INPUT_PU);

	sw[0].setup(PB4,INPUT_PU);
	sw[1].setup(PB5,INPUT_PU);

	limit[0].setup(PA4,INPUT_PU);
	limit[1].setup(PA5,INPUT_PU);
	limit[2].setup(PB0,INPUT_PU);
	limit[3].setup(PB1,INPUT_PU);

	en[0].pwmSetup(TIM1,2,PA9,PWM_PERIOD);
	en[1].pwmSetup(TIM1,1,PA8,PWM_PERIOD);

	pwm[0].pwmSetup(TIM4,3,PB8,PWM_PERIOD);
	pwm[1].pwmSetup(TIM4,4,PB9,PWM_PERIOD);
	pwm[2].pwmSetup(TIM4,1,PB6,PWM_PERIOD);
	pwm[3].pwmSetup(TIM4,2,PB7,PWM_PERIOD);

	motor[0].setup(pwm[0],pwm[1]);
	motor[1].setup(pwm[2],pwm[3]);
	motor[0].enPwmAssigne(en[0]);
	motor[1].enPwmAssigne(en[1]);

	enc[0].encoderSetup(TIM2,PA0,PA1);
	enc[1].encoderSetup(TIM3,PA6,PA7);

	//ここまでピンの設定

	if(sw[0].gpioRead() == 0){
		debugMode = 1;
	}else if(sw[1].gpioRead() == 0){
		debugMode = 2;
	}
	debugMode = 1;							//デバッグモード入れる

	//sys.usartSetup(serial);

	can1.setup(CAN1,PA12,PA11);
	if(debugMode)can1.debug();

	canMD[0].setup(motor[0],can1,(CAN_ADDRESS * 2));
	canMD[1].setup(motor[1],can1,(CAN_ADDRESS * 2) + 1);

	canEnc[0].setup(enc[0],can1,(CAN_ADDRESS * 2));
	canEnc[1].setup(enc[1],can1,(CAN_ADDRESS * 2) + 1);

	canSw.setup(limit[0],can1,(CAN_ADDRESS * 2));
	canSw.pinAdd(limit[1]);
	canSw.pinAdd(limit[2]);
	canSw.pinAdd(limit[3]);

	canVol.setup(isoIn,2,can1,0);
	canVoltage.setup(can1,0,10);


	if(debugMode){
		canEncoder[0].setup(can1,(CAN_ADDRESS * 2),10);
		canEncoder[1].setup(can1,(CAN_ADDRESS * 2) + 1,10);

		canMotor[0].setup(can1,(CAN_ADDRESS * 2));
		canMotor[1].setup(can1,(CAN_ADDRESS * 2) + 1);

		canSwitch.setup(can1,CAN_ADDRESS,100);

	}else{
		canMD[0].ledAssign(led[0]);
		canMD[1].ledAssign(led[1]);
	}

	serial.setup(USART3,921600,PB10,PB11);
	serial.printf("\n\rFILE = %s\n\r",__FILE__);
	serial.printf("DATE = %s\n\r",__DATE__);
	serial.printf("TIME = %s\n\r",__TIME__);
	serial.printf("ADRS = %d\n\r",CAN_ADDRESS);

	iso.setup(USART2,234000,PA2,PA3);
	isoIn.setup(iso,SI8900_MODE_LOOP);


	while(sw[0].gpioRead() == 0 || sw[1].gpioRead() == 0);

	serial.printf("setup end ");
	if(debugMode == 1){
		serial.printf("mode : debug-motor\n\r");
		led[0].write(1);
	}else if(debugMode == 2){
		serial.printf("mode : debug-sensor\n\r");
		led[1].write(1);
	}else{
		serial.printf("mode : run\n\r");
	}
}

extern "C" void USB_LP_CAN1_RX0_IRQHandler(void){
	can1.receive();
	canMD[0].interrupt();
	canMD[1].interrupt();

	canEnc[0].interrupt();
	canEnc[1].interrupt();

	canVoltage.interrupt();
	canVol.interrupt();

	canSw.interrupt();

	if(debugMode != 0){
		canEncoder[1].interrupt();
		canEncoder[0].interrupt();

		canSwitch.interrupt();
	}
	return;
}

#endif
