#include "stm32f10x.h"
#include "stdio.h"


#include "system.h"
#include "app.h"

#define CAN_VLV 0x280
#define CAN_ENC 0x400
#define CAN_MTD 0x100

#define IntervalTime 100

#define PWM_PERIOD 2048

#define Kp (long int)5
#define Kd (long int)10

uint16_t rxFlag = 0,encOld;

long int encPwm;

uint8_t canData[8] = {0,0,0,0,0,0,0,0},canAddress=0;

uint64_t intervalTimer = 0,timCnt=0;

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


	setup();



	timer.timerSetup(TIM1);

	encAa.setup(PA0,GPIO_Mode_IPU);
	encAb.setup(PA1,GPIO_Mode_IPU);
	encBa.setup(PA6,GPIO_Mode_IPU);
	encBb.setup(PA7,GPIO_Mode_IPU);
	encA.encoderSetup(TIM2);
	encB.encoderSetup(TIM3);

	sw1.setup(PB4,GPIO_Mode_IPU);
	sw2.setup(PB5,GPIO_Mode_IPU);

	ledA.setup(PA15,GPIO_Mode_Out_PP);
	ledB.setup(PB3,GPIO_Mode_Out_PP);
	buzzer.setup(PB2,GPIO_Mode_Out_PP);
	ledA.write(Bit_RESET);
	ledB.write(Bit_RESET);

	sel1.setup(PB12,GPIO_Mode_IPU);
	sel2.setup(PB13,GPIO_Mode_IPU);
	sel4.setup(PB14,GPIO_Mode_IPU);
	sel8.setup(PB15,GPIO_Mode_IPU);
	canAddress = 15 - (sel1.read()*4 + sel2.read()*1 + sel4.read()*2 + sel8.read()*8);

	io0.setup(PB0,GPIO_Mode_Out_PP);
	io1.setup(PB1,GPIO_Mode_Out_PP);
	io2.setup(PA9,GPIO_Mode_Out_PP);
	io3.setup(PA10,GPIO_Mode_Out_PP);


	motorEN.setup(PA8,GPIO_Mode_Out_PP);

	limitA1.setup(PA2,GPIO_Mode_Out_PP);
	limitA2.setup(PA3,GPIO_Mode_Out_PP);
	limitB1.setup(PA4,GPIO_Mode_Out_PP);
	limitB2.setup(PA5,GPIO_Mode_Out_PP);

	usartTX.setup(PB10,GPIO_Mode_AF_PP);
	usartRX.setup(PB11,GPIO_Mode_IN_FLOATING);

	canTX.setup(PA12,GPIO_Mode_AF_PP);
	canRX.setup(PA11,GPIO_Mode_IN_FLOATING);

	pinPwmA.setup(PB8,GPIO_Mode_AF_PP);
	pinPwmAN.setup(PB9,GPIO_Mode_AF_PP);
	pinPwmB.setup(PB6,GPIO_Mode_AF_PP);
	pinPwmBN.setup(PB7,GPIO_Mode_AF_PP);

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
	serial.printf("\n\roppai\n\r");
	serial.printf("FILE = %s\n\r",__FILE__);
	serial.printf("DATE = %s\n\r",__DATE__);
	serial.printf("TIME = %s\n\r",__TIME__);
	serial.printf("ADRS = %d\n\r",canAddress);


	delay(100);

	CAN1Setup();


    while(1){
    	canAddress = 15 - (sel1.read()*4 + sel2.read()*1 + sel4.read()*2 + sel8.read()*8);

    	if(sw2.read() == 0){
    		ledB.write(Bit_SET);
    		serial.printf("B\n\r");
    	}else{
    		ledB.write(Bit_RESET);
    	}


    	if(sw1.read() == 0){
    		ledA.write(Bit_SET);
    		serial.printf("A\n\r");
    	}else{
    		ledA.write(Bit_RESET);
    	}

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

extern "C" void TIM1_UP_IRQHandler(void){
	timCnt++;
	TIM_ClearITPendingBit(TIM1,TIM_IT_Update);
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);
}
