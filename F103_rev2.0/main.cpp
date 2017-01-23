#include "stm32f10x.h"
#include "stdio.h"


#include "system.h"
#include "app.h"

#define CAN_VLV 0x280
#define CAN_ENC_SET 0x400
#define CAN_ENC_VAL 0x440
#define CAN_MTD 0x104

#define PWM_PERIOD 2048

#define Kp (long int)5
#define Kd (long int)10

#define TEST_DUTY 200

#define MODE_RUN 0
#define MODE_DEBUG 1

uint16_t rxFlag = 0,encOld;

long int encPwm;

uint8_t canData[8] = {0,0,0,0,0,0,0,0},canAddress=0;

union UNION{
	uint8_t can8bit[4];
	int32_t enc32bit;
};
int32_t enc[4];

int32_t outA_duty = 0,outB_duty = 0;
uint64_t motorTimeA,motorTimeB;

uint64_t intervalTime = 0,timCnt=0;

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

GPIO pinPwmA;
GPIO pinPwmAN;
GPIO pinPwmB;
GPIO pinPwmBN;

GPIO pin13;
GPIO pin14;
GPIO pin15;

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

CAN can1;

uint8_t motorModeA = MODE_RUN,motorModeB = MODE_RUN;

int main(void)
{


	union UNION encValue;
	encValue.enc32bit = 0;

	setup();

	timer.timerSetup(TIM1);

	encAa.setup(PA0,INPUT_PU);
	encAb.setup(PA1,INPUT_PU);
	encBa.setup(PA6,INPUT_PU);
	encBb.setup(PA7,INPUT_PU);
	encA.encoderSetup(TIM2);
	encB.encoderSetup(TIM3);

	sw1.setup(PA2,INPUT_PU);
	sw2.setup(PA3,INPUT_PU);

	pin13.setup(PC13,OUTPUT);
	pin14.setup(PC14,OUTPUT);
	pin15.setup(PC15,OUTPUT);

	ledA.setup(PA4,OUTPUT);
	ledB.setup(PA5,OUTPUT);
	buzzer.setup(PB0,OUTPUT);
	ledA.write(Bit_RESET);
	ledB.write(Bit_RESET);

	sel1.setup(PB2,INPUT_PU);
	sel2.setup(PB10,INPUT_PU);
	sel4.setup(PB1,INPUT_PU);
	sel8.setup(PB11,INPUT_PU);
	canAddress = 15 - (sel1.read() + sel2.read()*2 + sel4.read()*4 + sel8.read()*8);

	motorEN.setup(PB12,OUTPUT);

	limitA1.setup(PB13,OUTPUT);
	limitA2.setup(PB14,OUTPUT);
	limitB1.setup(PB15,OUTPUT);
	limitB2.setup(PA8,OUTPUT);

	usartTX.setup(PA9,OUTPUT_AF);
	usartRX.setup(PA10,INPUT);

	canTX.setup(PA12,OUTPUT_AF);
	canRX.setup(PA11,INPUT);

	pinPwmA.setup(PB8,OUTPUT_AF);
	pinPwmAN.setup(PB9,OUTPUT_AF);
	pinPwmB.setup(PB6,OUTPUT_AF);
	pinPwmBN.setup(PB7,OUTPUT_AF);

	pinPwmA.write(LOW);
	pinPwmAN.write(LOW);
	pinPwmB.write(LOW);
	pinPwmBN.write(LOW);
	motorEN.write(HIGH);
	delay(100);
	motorEN.write(LOW);

	pwmA.pwmSetup(TIM4,3,PWM_PERIOD);
	pwmAN.pwmSetup(TIM4,4,PWM_PERIOD);
	pwmB.pwmSetup(TIM4,1,PWM_PERIOD);
	pwmBN.pwmSetup(TIM4,2,PWM_PERIOD);

	motorA.setup(pwmA,pwmAN);
	motorB.setup(pwmB,pwmBN);

	pwmA.duty(PWM_PERIOD/2);
	pwmAN.duty(PWM_PERIOD/2);
	pwmB.duty(PWM_PERIOD/2);
	pwmBN.duty(PWM_PERIOD/2);
	motorEN.write(Bit_SET);

	serial.setup(USART1,921600);
	serial.printf("\n\rFILE = %s\n\r",__FILE__);
	serial.printf("DATE = %s\n\r",__DATE__);
	serial.printf("TIME = %s\n\r",__TIME__);
	serial.printf("ADRS = %d\n\r",canAddress);

	if(sw1.read() == 0){
		motorModeA = MODE_DEBUG;
		serial.printf("motorA debug mode\n\rsw1:cw sw2 ccw\n\r");
	}
	if(sw2.read() == 0){
		motorModeB = MODE_DEBUG;
		serial.printf("motorB debug mode\n\rsw1:cw sw2 ccw\n\r");
	}

	while(sw1.read() == 0 || sw2.read() == 0);
	delay(500);

	can1.setup();
	can1.filterAdd(CAN_MTD,CAN_MTD + 1);

	motorTimeA = 0;
	motorTimeB = 0;

    while(1){
    	//canAddress = 15 - (sel1.read() + sel2.read()*2 + sel4.read()*4 + sel8.read()*8);


    	if(motorTimeA + 100 < millis()){
    		motorA.duty(0);
    	}
    	if(motorTimeB + 100 < millis()){
    		motorB.duty(0);
    	}

    	if(sw1.read() == 0){
    		if(motorModeA == MODE_DEBUG){
    			motorA.duty(TEST_DUTY);
    			motorTimeA = millis();
    		}
    		if(motorModeB == MODE_DEBUG){
    			motorTimeB = millis();
    			motorB.duty(TEST_DUTY);
    		}
    	}else if(sw2.read() == 0){
    		if(motorModeA == MODE_DEBUG){
    			motorA.duty(-TEST_DUTY);
    			motorTimeA = millis();
    		}
    		if(motorModeB == MODE_DEBUG){
    			motorTimeB = millis();
    			motorB.duty(-TEST_DUTY);
    		}
    	}

    	while(rxFlag > 0){
    		rxFlag--;
    		//serial.printf("CAN ADD = %x,Data = %d,%d,%d,%d,%d,%d,%d,%d\n\r",RxMessage.StdId,RxMessage.Data[0],RxMessage.Data[1],RxMessage.Data[2],RxMessage.Data[3],RxMessage.Data[4],RxMessage.Data[5],RxMessage.Data[6],RxMessage.Data[7]);
    	}

    }
}


extern "C" void USB_LP_CAN1_RX0_IRQHandler(void){
	rxFlag++;
	//can1.receive(&RxMessage);

	CAN_Receive(CAN1,CAN_FIFO0,&RxMessage);




	if(RxMessage.StdId == CAN_MTD){
		outA_duty = 0;
		motorTimeA = millis();

		outA_duty = ((RxMessage.Data[1] & 0b01111111) << 8) | RxMessage.Data[0];
		if((RxMessage.Data[1] >> 7) == 1){
			outA_duty = -outA_duty;
		}

		outA_duty /= 32;

		motorA.duty(outA_duty);
	}else if(RxMessage.StdId == CAN_MTD + 1){
		outB_duty = 0;
		motorTimeB = millis();

		outB_duty = ((RxMessage.Data[1] & 0b01111111) << 8) | RxMessage.Data[0];
		if((RxMessage.Data[1] >> 7) == 1){
			outB_duty = -outB_duty;
		}

		outB_duty /= 32;
		motorB.duty(outB_duty);
	}




	serial.printf("A = %d,B = %d\n\r",outA_duty,outB_duty);
	return;
}
