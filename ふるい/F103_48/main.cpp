#include "stm32f10x.h"
#include "stdio.h"


#include "system.h"
#include "app.h"

#define CAN_VLV 0x280
#define CAN_ENC 0x400
#define CAN_MTD 0x100

#define CYCLE_TIME 1000

#define PWM_PERIOD 5000

#define Kp (long int)5
#define Kd (long int)10

#define ADC_TO_CURRENT (float)0.24.4140625

uint16_t rxFlag = 0,encOld;

long int encPwm;

uint8_t canData[8] = {0,0,0,0,0,0,0,0},canAddress=0;

uint64_t cycleTime = 0,timCnt=0;

CanRxMsg RxMessage;

GPIO encAa;
GPIO encAb;
GPIO encBa;
GPIO encBb;

GPIO sw1;
GPIO sw2;

GPIO buzzer;
GPIO ledA;
GPIO ledB;

GPIO sel1;
GPIO sel2;
GPIO sel4;
GPIO sel8;

GPIO motorEN;
GPIO limitA1;
GPIO limitA2;
GPIO limitB1;
GPIO limitB2;

GPIO usartTX;
GPIO usartRX;
GPIO canTX;
GPIO canRX;

GPIO io0;
GPIO io1;
GPIO io2;
GPIO io3;

ADC cs[2];

GPIO pinPwmA;
GPIO pinPwmAN;
GPIO pinPwmB;
GPIO pinPwmBN;

TIM timer;

TIM pwmA;
TIM pwmAN;
TIM pwmB;
TIM pwmBN;

TIM encA;
TIM encB;

USART serial;

MOTOR motorA;
MOTOR motorB;



int main(void)
{
	uint8_t csCnt[2] = {0,0};
	int32_t csAve[2] = {0,0},targetCurrent[2];
	uint32_t csOffset[2] = {0,0};

	uint16_t current[100];

	int32_t motorDuty[2];
	int32_t prop[2] = {0,0},diff[2] = {0,0},intg[2] = {0,0};

	setup();
	timer.timerSetup(TIM1);

	encAa.setup(PA0,INPUT_PU);
	encAb.setup(PA1,INPUT_PU);
	encBa.setup(PA6,INPUT_PU);
	encBb.setup(PA7,INPUT_PU);
	encA.encoderSetup(TIM2);
	encB.encoderSetup(TIM3);

	sw1.setup(PB4,INPUT_PU);
	sw2.setup(PB5,INPUT_PU);

	ledA.setup(PA15,OUTPUT);
	ledB.setup(PB3,OUTPUT);
	buzzer.setup(PB2,OUTPUT);
	ledA.write(Bit_RESET);
	ledB.write(Bit_RESET);


	sel1.setup(PB14,INPUT_PU);
	sel2.setup(PB15,INPUT_PU);
	sel4.setup(PB12,INPUT_PU);
	sel8.setup(PB13,INPUT_PU);
	canAddress = 15 - (sel8.read()*8 + sel4.read()*4 + sel2.read()*2 + sel1.read()*1);

	io0.setup(PB0,INPUT_AIN);
	io1.setup(PB1,INPUT_AIN);
	io2.setup(PA9,INPUT);
	io3.setup(PA10,INPUT);

	cs[0].setup(ADC2,8);
	cs[1].setup(ADC2,9);

	motorEN.setup(PA8,OUTPUT);

	limitA1.setup(PA2,OUTPUT);
	limitA2.setup(PA3,OUTPUT);
	limitB1.setup(PA4,OUTPUT);
	limitB2.setup(PA5,OUTPUT);

	usartTX.setup(PB10,OUTPUT_AF);
	usartRX.setup(PB11,INPUT);

	canTX.setup(PA12,OUTPUT_AF);
	canRX.setup(PA11,INPUT);

	pinPwmA.setup(PB8,OUTPUT_AF);
	pinPwmAN.setup(PB9,OUTPUT_AF);
	pinPwmB.setup(PB6,OUTPUT_AF);
	pinPwmBN.setup(PB7,OUTPUT_AF);

	pinPwmA.write(Bit_RESET);
	pinPwmAN.write(Bit_RESET);
	pinPwmB.write(Bit_RESET);
	pinPwmBN.write(Bit_RESET);

	motorEN.write(Bit_SET);
	delay(100);
	motorEN.write(Bit_RESET);


	pwmA.pwmSetup(TIM4,3,PWM_PERIOD,TIM_OCMode_PWM1);
	pwmAN.pwmSetup(TIM4,4,PWM_PERIOD,TIM_OCMode_PWM1);
	pwmB.pwmSetup(TIM4,1,PWM_PERIOD,TIM_OCMode_PWM1);
	pwmBN.pwmSetup(TIM4,2,PWM_PERIOD,TIM_OCMode_PWM1);

	motorA.setup(pwmA,pwmAN);
	motorB.setup(pwmB,pwmBN);

	pwmA.duty(PWM_PERIOD/2);
	pwmAN.duty(PWM_PERIOD/2);
	pwmB.duty(PWM_PERIOD/2);
	pwmBN.duty(PWM_PERIOD/2);

	motorEN.write(Bit_SET);

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);

	serial.setup(USART3,921600);
	serial.printf("FILE = %s\n\r",__FILE__);
	serial.printf("DATE = %s\n\r",__DATE__);
	serial.printf("TIME = %s\n\r",__TIME__);
	serial.printf("ADRS = %d\n\r",canAddress);


	delay(1000);

	CAN1Setup();


	while(1){
    	if(csCnt[0] < 3){
    		csCnt[0]++;
    		csAve[0] += cs[0].read();
    		csAve[1] += cs[1].read();
    	}else{
    		csOffset[0] = csAve[0];
    		csOffset[1] = csAve[1];
    		break;
    	}
	}



	motorDuty[0] = 0;
	motorDuty[1] = 0;

	serial.printf("%6d\n\r",cs[1].read());
	motorA.duty(PWM_PERIOD/2);
	serial.printf("t1 = %d\n\r",micros());
	for(int m=0;m<100;m++){
		current[m] = cs[1].read();
	}
	serial.printf("t0 = %d\n\r",micros());
	motorA.duty(0);
	for(int m=0;m<100;m++){
		current[m] -=2048;
		serial.printf("%6d\r\n",(current[m] * 49) / 2);
		delay(1);
	}

	while(1);

    while(1){

    	if(csCnt[0] < 3){
    		csCnt[0]++;
    		//csAve[0] += cs[0].read();
    		csAve[1] += cs[1].read();
    	}else{
    		csCnt[0] = 0;
    		csAve[0] -= csOffset[0];
    		csAve[0] /= 10;
    		csAve[0] = csAve[0] * 34;

    		csAve[1] -= csOffset[1];
    		csAve[1] /= 10;
    		csAve[1] = csAve[1] * 34;

    		serial.printf("\n\r%6d,mA,,",csAve[1]);

    		if(sw1.read() == 0){
    			targetCurrent[0] = 10000;
    		}else if(sw2.read() == 0){
    			targetCurrent[0] = -10000;
    		}else{
    			targetCurrent[0] = 0;
    		}

    		diff[0] = -prop[0];
    		prop[0] = targetCurrent[0] + csAve[1];
    		diff[0] += prop[0];
    		intg[0] -= prop[0];

    		//motorDuty[0] = diff[0]/5 + prop[0] + intg[0]/5;

    		motorDuty[0] = -prop[0] / 10 + intg[0]/5000;// - diff[0]/100;
    		motorA.duty(motorDuty[0]);


    		serial.printf("%4d,%4d,%4d,%4d",diff[0],prop[0],intg[0],motorDuty[0]);

    		csAve[0] = 0;
    		csAve[1] = 0;
    	}


    	if(micros() > cycleTime){

    	}

    	canAddress = 15 - (sel1.read()*4 + sel2.read()*1 + sel4.read()*2 + sel8.read()*8);


    	while(rxFlag > 0){
    		rxFlag--;
    		if(RxMessage.StdId == 0x0103){
    			int32_t outA_duty = 0;
    			motorA.duty(outA_duty/31);
    			serial.printf("OUTA = %d,",outA_duty/31);
    		}
    		serial.printf("CAN ADD = %x,Data = %d,%d,%d,%d,%d,%d,%d,%d\n\r",RxMessage.StdId,RxMessage.Data[0],RxMessage.Data[1],RxMessage.Data[2],RxMessage.Data[3],RxMessage.Data[4],RxMessage.Data[5],RxMessage.Data[6],RxMessage.Data[7]);
    	}

    }
}


extern "C" void USB_LP_CAN1_RX0_IRQHandler(void){
	rxFlag++;
	CAN_Receive(CAN1,CAN_FIFO0,&RxMessage);

	return;
}

extern "C" void CAN1_RX1_IRQHandler(void){
	rxFlag++;
	CAN_Receive(CAN1,CAN_FIFO0,&RxMessage);
	return;
}
