#ifndef MAIN_H_
#define MAIN_H_

#include "system.h"
#include "app.h"
#include "base.h"
#include "can_dcmd.h"

#define CAN_ADDRESS (uint16_t)(15 - (sel[0].read()*2 + sel[1].read() + sel[2].read()*8 + sel[3].read()*4))
#define IntervalTime 100
#define PWM_PERIOD 2000

#define BOARD_ADD_SAGI0 0
#define BOARD_ADD_SAGI1 3
#define BOARD_ADD_SHOT  2
#define BOARD_ADD_ANGLE 1	//仮

System sys;

GPIO led[4],sel[4],adcRst,adcPowerOn,currentOption;
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
CanNodeEmg canEmg;

CanMotorDriver canMotor[2];
CanEncoder canEncoder[2];
CanSwitch canSwitch;
CanVoltage canVoltage;

CanDCMD driver;

uint8_t debugMode = 0,printValue = 0,canData[8];
uint64_t intervalTimer = 0;


void setup(){
	sys.setup();

	serial.setup(USART3,115200,PB10,PB11);
	serial.printf("\n\rFILE = %s\n\r",__FILE__);
	serial.printf("DATE = %s\n\r",__DATE__);
	serial.printf("TIME = %s\n\r",__TIME__);

	delay(100);

	led[0].setup(PA15,OUTPUT);
	led[1].setup(PB3,OUTPUT);
	led[2].setup(PC13,OUTPUT);
	led[3].setup(PC14,OUTPUT);

	currentOption.setup(PC15,INPUT_PU);

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
	en[0].duty(0);
	en[1].duty(0);

	pwm[0].pwmSetup(TIM4,3,PB8,PWM_PERIOD);
	pwm[1].pwmSetup(TIM4,4,PB9,PWM_PERIOD);
	pwm[2].pwmSetup(TIM4,1,PB6,PWM_PERIOD);
	pwm[3].pwmSetup(TIM4,2,PB7,PWM_PERIOD);

	motor[0].setup(pwm[0],pwm[1]);
	motor[1].setup(pwm[2],pwm[3]);
	motor[0].enPwmAssigne(en[0]);
	motor[1].enPwmAssigne(en[1]);

	adcRst.setup(PA10,OUTPUT);
	adcPowerOn.setup(PB2,INPUT);

	iso.setup(USART2,234000,PA2,PA3);
	isoIn.setup(iso,SI8900_MODE_LOOP);
	isoIn.gpioAssigne(adcPowerOn,adcRst);

	if(sw[0].gpioRead() == 0){
		debugMode = 1;
	}else if(sw[1].gpioRead() == 0){
		debugMode = 2;
	}

	can1.setup(CAN1,PA12,PA11,debugMode);

	enc[0].encoderSetup(TIM2,PA0,PA1);
	enc[1].encoderSetup(TIM3,PA6,PA7);

	//ここまでピンの設定


	canEnc[0].setup(enc[0],can1,(CAN_ADDRESS * 2) + 0x10);
	canEnc[1].setup(enc[1],can1,(CAN_ADDRESS * 2) + 0x11);

	canSw.setup(limit[0],can1,(CAN_ADDRESS * 2));		//リミット
	canSw.pinAdd(limit[1]);
	canSw.pinAdd(limit[2]);
	canSw.pinAdd(limit[3]);

	canVol.setup(isoIn,2,AdcToVoltageGain,can1,CAN_ADDRESS*2);			//電源電圧｡とりあえずMDのほうと合わせておく

	canMD[0].setup(motor[0],can1,(CAN_ADDRESS * 2));
	canMD[1].setup(motor[1],can1,(CAN_ADDRESS * 2) + 1);
	canMD[0].ledAssign(led[0]);
	canMD[1].ledAssign(led[2]);		//白LEDだけ

	canEmg.setup(can1,CAN_ADDRESS * 2);

	driver.canmdSetup(canMD[0],canMD[1],CAN_ADDRESS);
	driver.adcSetup(isoIn);
	driver.emgSetup(canEmg);
	driver.optionSetup(currentOption);

	if(debugMode){

		canVoltage.setup(can1,CAN_ADDRESS*2,100);

		canEncoder[0].setup(can1,(CAN_ADDRESS * 2),10);
		canEncoder[1].setup(can1,(CAN_ADDRESS * 2) + 1,10);

		canMotor[0].setup(can1,(CAN_ADDRESS * 2));
		canMotor[1].setup(can1,(CAN_ADDRESS * 2) + 1);

		canSwitch.setup(can1,CAN_ADDRESS*2,100);
	}

	while(sw[0].gpioRead() == 0 || sw[1].gpioRead() == 0);

	serial.printf("hw option = %d,",driver.hardwareOption);	//ハードウェア識別
	if(driver.hardwareOption == 1){
		serial.printf("high end model\n\r");
	}else{
		serial.printf("mid-range model\n\r");
	}
/*
	if(CAN_ADDRESS == BOARD_ADD_SAGI0 || CAN_ADDRESS == BOARD_ADD_SAGI1){
		driver.overCurrentPeakSet(0,170);
		driver.overCurrentPeakSet(1,170);

		driver.overCurrentAveSet(0,170);
		driver.overCurrentAveSet(1,170);
	}

	if(CAN_ADDRESS == BOARD_ADD_SHOT){
		driver.overCurrentPeakSet(ChannelCurrentA,40);
		driver.overCurrentPeakSet(ChannelCurrentB,150);

		driver.overCurrentAveSet(ChannelCurrentA,30);
		driver.overCurrentAveSet(ChannelCurrentB,100);

	}else if(CAN_ADDRESS == BOARD_ADD_ANGLE){
		driver.overCurrentPeakSet(0,150);
		driver.overCurrentPeakSet(1,150);

		driver.overCurrentAveSet(0,20);
		driver.overCurrentAveSet(1,20);
	}
	*/

	/*
	if(CAN_ADDRESS == BOARD_ADD_SAGI0 || CAN_ADDRESS == BOARD_ADD_SAGI1){
		driver.overCurrentPeakSet(0,170);
		driver.overCurrentPeakSet(1,170);

		driver.overCurrentAveSet(0,120);
		driver.overCurrentAveSet(1,120);
	}

	if(CAN_ADDRESS == BOARD_ADD_SHOT){
		driver.overCurrentPeakSet(ChannelCurrentA,50);	//上射出
		driver.overCurrentPeakSet(ChannelCurrentB,50);

		driver.overCurrentAveSet(ChannelCurrentA,40);
		driver.overCurrentAveSet(ChannelCurrentB,40);

	}else if(CAN_ADDRESS == BOARD_ADD_ANGLE){
		driver.overCurrentPeakSet(ChannelCurrentA,150);	//仰角
		driver.overCurrentPeakSet(ChannelCurrentB,150);	//下射出

		driver.overCurrentAveSet(ChannelCurrentA,20);
		//driver.overCurrentAveSet(ChannelCurrentB,100);
		driver.overCurrentAveSet(ChannelCurrentB,120);
	}*/

	driver.overCurrentPeakSet(ChannelCurrentA,180);	//150
	driver.overCurrentPeakSet(ChannelCurrentB,180);

	driver.overCurrentAveSet(ChannelCurrentA,80);//50
	driver.overCurrentAveSet(ChannelCurrentB,80);

	serial.printf("OC limit max A=%3d,B=%3d,ave A=%3d,B=%3d\n\r",(int)driver.overCurrentLimit[0],(int)driver.overCurrentLimit[1],(int)driver.overCurrentLimitAve[0],(int)driver.overCurrentLimitAve[1]);

	delay(100);
	serial.printf("ADRS = %d\n\r",CAN_ADDRESS);
	serial.printf(" md add = %#x,%#x\n\renc add = %#x,%#x\n\r sw add = %#x\n\r",canMD[0].canMd_address[0],canMD[1].canMd_address[0],canEnc[0].canEnc_address,canEnc[1].canEnc_address,canSw.canAddress);
	delay(100);

	serial.printf("setup end ");
	if(debugMode == 1){
		serial.printf("mode : debug-motor\n\r");
	}else if(debugMode == 2){
		serial.printf("mode : debug-sensor\n\r");
	}else{
		serial.printf("mode : run\n\r");
	}

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
}



extern "C" void USB_LP_CAN1_RX0_IRQHandler(void){
	can1.receive();
	canMD[0].interrupt();
	canMD[1].interrupt();

	canEnc[0].interrupt();
	canEnc[1].interrupt();

	canVol.interrupt();

	canSw.interrupt();

	if(debugMode != 0){
		canEncoder[1].interrupt();
		canEncoder[0].interrupt();

		canVoltage.interrupt();

		canSwitch.interrupt();
	}

	return;
}

#endif
