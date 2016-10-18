#include "stm32f10x.h"
#include "stdio.h"


#include "SYSTEM.h"
#include "app.hpp"

#define CAN_VLV 0x280
#define CAN_ENC 0x400
#define CAN_MTD 0x100

#define IntervalTime 100

#define PWM_PERIOD 2048

#define Kp (long int)5
#define Kd (long int)10



uint16_t a,adcChannelStat = 1;
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

GPIO spiNSS;
GPIO spiSCK;
GPIO spiMOSI;
GPIO spiMISO;

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
	GPIOSetup();

	timer.timerSetup(TIM1);

	encAa.setup(GPIOA,GPIO_Pin_0,GPIO_Mode_IPU);
	encAb.setup(GPIOA,GPIO_Pin_1,GPIO_Mode_IPU);
	encBa.setup(GPIOA,GPIO_Pin_0,GPIO_Mode_IPU);
	encBb.setup(GPIOA,GPIO_Pin_1,GPIO_Mode_IPU);
	encA.encoderSetup(TIM2);
	encB.encoderSetup(TIM3);

	sw1.setup(GPIOA,GPIO_Pin_2,GPIO_Mode_IPU);
	sw2.setup(GPIOA,GPIO_Pin_3,GPIO_Mode_IPU);

	ledA.setup(GPIOA,GPIO_Pin_4,GPIO_Mode_Out_PP);
	ledB.setup(GPIOA,GPIO_Pin_5,GPIO_Mode_Out_PP);
	buzzer.setup(GPIOB,GPIO_Pin_0,GPIO_Mode_Out_PP);
	ledA.write(Bit_RESET);
	ledB.write(Bit_RESET);

	sel1.setup(GPIOB,GPIO_Pin_1,GPIO_Mode_IPU);
	sel2.setup(GPIOB,GPIO_Pin_2,GPIO_Mode_IPU);
	sel4.setup(GPIOB,GPIO_Pin_10,GPIO_Mode_IPU);
	sel8.setup(GPIOB,GPIO_Pin_11,GPIO_Mode_IPU);
	canAddress = 15 - (sel1.read()*4 + sel2.read()*1 + sel4.read()*2 + sel8.read()*8);

	motorEN.setup(GPIOB,GPIO_Pin_12,GPIO_Mode_Out_PP);

	limitA1.setup(GPIOB,GPIO_Pin_13,GPIO_Mode_Out_PP);
	limitA2.setup(GPIOB,GPIO_Pin_14,GPIO_Mode_Out_PP);
	limitB1.setup(GPIOB,GPIO_Pin_15,GPIO_Mode_Out_PP);
	limitB2.setup(GPIOA,GPIO_Pin_8,GPIO_Mode_Out_PP);

	usartTX.setup(GPIOA,GPIO_Pin_9,GPIO_Mode_AF_PP);
	usartTX.setup(GPIOA,GPIO_Pin_10,GPIO_Mode_IN_FLOATING);

	canTX.setup(GPIOA,GPIO_Pin_12,GPIO_Mode_AF_PP);
	canRX.setup(GPIOA,GPIO_Pin_11,GPIO_Mode_AF_PP);

	/*
	spiNSS.setup(GPIOA,GPIO_Pin_15,GPIO_Mode_IPU);
	spiSCK.setup(GPIOB,GPIO_Pin_3,GPIO_Mode_AF_PP);
	spiMOSI.setup(GPIOB,GPIO_Pin_4,GPIO_Mode_AF_PP);
	spiMISO.setup(GPIOB,GPIO_Pin_5,GPIO_Mode_AF_PP);
*/

	pinPwmA.setup(GPIOB,GPIO_Pin_8,GPIO_Mode_AF_PP);
	pinPwmAN.setup(GPIOB,GPIO_Pin_9,GPIO_Mode_AF_PP);
	pinPwmB.setup(GPIOB,GPIO_Pin_6,GPIO_Mode_AF_PP);
	pinPwmBN.setup(GPIOB,GPIO_Pin_7,GPIO_Mode_AF_PP);

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


	serial.setup(USART1,921600);
	serial.printf("FILE = %s\n\r",__FILE__);
	serial.printf("DATE = %s\n\r",__DATE__);
	serial.printf("TIME = %s\n\r",__TIME__);
	serial.printf("ADRS = %d\n\r",canAddress);

	delay(100);

	TIM4ITSetup(TIM_IT_CC1);

	CAN1Setup();


    while(1){

    	canAddress = 15 - (sel1.read()*4 + sel2.read()*1 + sel4.read()*2 + sel8.read()*8);

    	//serial.printf("%4d,%4d,%6d,%6d,%6d\n\r",encA.read(),encB.read(),motorB.outDuty,motorB.outDuty,timCnt);

    	int outA = encA.read();
    	motorA.duty(outA);
    	motorB.duty(outA);

    	if(sw2.read() == 0){
    		ledB.write(Bit_SET);
    	}else{
    		ledB.write(Bit_RESET);
    	}

    	if(sw1.read() == 0){
    		ledA.write(Bit_SET);
    	}else{
    		ledA.write(Bit_RESET);
    	}


    	outA = encA.read()*50;
    	if(outA > 0x7FFF)outA = 0x7FFF;
    	if(outA < -0x8000)outA = -0x8000;
    	if(outA < 0)outA = ~outA + 0x8000;
    	canData[0] = outA & 0xFF;
    	canData[1] = outA>>8;
    	if(canAddress != 0){
    		//CAN1Send(CAN_MTD + canAddress - 1,2,canData);
    	}
    	canData[0] = 0xFF;
    	canData[1] = canAddress + (canAddress << 4);
    	if(canAddress != 0){
    		CAN1Send(CAN_VLV,2,canData);
    	}

    	delay(10);
    	//ledA.toggle();


    	while(rxFlag > 0){
    		rxFlag--;
    		serial.printf("CAN ADD = %x,Data = %d,%d,%d,%d,%d,%d,%d,%d\n\r",RxMessage.StdId,RxMessage.Data[0],RxMessage.Data[1],RxMessage.Data[2],RxMessage.Data[3],RxMessage.Data[4],RxMessage.Data[5],RxMessage.Data[6],RxMessage.Data[7]);
    	}

    }
}


extern "C" void USB_LP_CAN1_RX0_IRQHandler(void){
	rxFlag++;
	CAN_Receive(CAN1,CAN_FIFO0,&RxMessage);
	return;
}

extern "C" void TIM4_IRQHandler(void){

	/*
	 * ADCのPWM同期をすると､mainループが著しく遅くなる｡
	 * TIM割込みには問題がない様子10/12
	 *
	 *
	 */
	if(TIM_GetITStatus(TIM4,TIM_IT_CC1) == SET){
		//currentA.peek();
		//currentA.start(ADC_SampleTime_13Cycles5);

		TIM_ITConfig(TIM4,TIM_IT_CC1,DISABLE);
		TIM_ClearITPendingBit(TIM4,TIM_IT_CC1);
		TIM_ITConfig(TIM4,TIM_IT_CC2,ENABLE);

	}else if(TIM_GetITStatus(TIM4,TIM_IT_CC2) == SET){
		//currentA.peek();
		//currentB.start(ADC_SampleTime_13Cycles5);

		TIM_ITConfig(TIM4,TIM_IT_CC2,DISABLE);
		TIM_ClearITPendingBit(TIM4,TIM_IT_CC2);
		TIM_ITConfig(TIM4,TIM_IT_CC3,ENABLE);

	}else if(TIM_GetITStatus(TIM4,TIM_IT_CC3) == SET){
		//currentB.peek();
		//currentB.start(ADC_SampleTime_13Cycles5);

		TIM_ITConfig(TIM4,TIM_IT_CC3,DISABLE);
		TIM_ClearITPendingBit(TIM4,TIM_IT_CC3);
		TIM_ITConfig(TIM4,TIM_IT_CC4,ENABLE);

	}else if(TIM_GetITStatus(TIM4,TIM_IT_CC4) == SET){
		//currentB.peek();
		//currentA.start(ADC_SampleTime_13Cycles5);

		TIM_ITConfig(TIM4,TIM_IT_CC4,DISABLE);
		TIM_ClearITPendingBit(TIM4,TIM_IT_CC4);
		TIM_ITConfig(TIM4,TIM_IT_CC1,ENABLE);

	}

	TIM_ClearITPendingBit(TIM4,TIM_IT_CC1);
	TIM_ClearITPendingBit(TIM4,TIM_IT_CC2);
	TIM_ClearITPendingBit(TIM4,TIM_IT_CC3);
	TIM_ClearITPendingBit(TIM4,TIM_IT_CC4);
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
}

extern "C" void TIM1_UP_IRQHandler(void){
	timCnt++;
	TIM_ClearITPendingBit(TIM1,TIM_IT_Update);
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);
}
