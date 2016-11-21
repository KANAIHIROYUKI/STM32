#include "stm32f10x.h"
#include "stdio.h"


#include "SYSTEM.h"


#define CAN_ID 0x280

#define IntervalTime 100

uint16_t a,adcChannelStat = 1;
uint16_t rxFlag = 0,tim4itCnt=0,currentA_16,currentB_16;

float currentA_A=0,currentB_A=0;

uint8_t canData[8] = {0,0,0,0,0,0,0,0},canAddress=0;

uint64_t intervalTimer = 0;

CanRxMsg RxMessage;

GPIO enc1a;
GPIO enc1b;
GPIO enc2a;
GPIO enc2b;
GPIO enc3a;
GPIO enc3b;
GPIO enc4a;
GPIO enc4b;

GPIO io1;
GPIO io2;
GPIO io3;
GPIO io4;
GPIO io5;
GPIO io6;
GPIO io7;
GPIO io8;

GPIO led;

GPIO usartTX;
GPIO usartRX;
GPIO canTX;
GPIO canRX;

TIM enc1;
TIM enc2;
TIM enc3;
TIM enc4;

USART serial;

int main(void)
{
	setup();
	GPIOSetup();

	enc1a.setup(GPIOA,GPIO_Pin_0,GPIO_Mode_IPU);
	enc1b.setup(GPIOA,GPIO_Pin_1,GPIO_Mode_IPU);

	enc2a.setup(GPIOA,GPIO_Pin_6,GPIO_Mode_IPU);
	enc2b.setup(GPIOA,GPIO_Pin_7,GPIO_Mode_IPU);

	enc3a.setup(GPIOB,GPIO_Pin_8,GPIO_Mode_IPU);
	enc3b.setup(GPIOB,GPIO_Pin_9,GPIO_Mode_IPU);

	enc4a.setup(GPIOB,GPIO_Pin_6,GPIO_Mode_IPU);
	enc4b.setup(GPIOB,GPIO_Pin_7,GPIO_Mode_IPU);

	enc1.encoderSetup(TIM1);
	enc2.encoderSetup(TIM2);
	enc3.encoderSetup(TIM3);
	enc4.encoderSetup(TIM4);

	io1.setup(GPIOA,GPIO_Pin_2,GPIO_Mode_IPU);
	io2.setup(GPIOA,GPIO_Pin_3,GPIO_Mode_IPU);
	io3.setup(GPIOA,GPIO_Pin_4,GPIO_Mode_IPU);
	io4.setup(GPIOA,GPIO_Pin_5,GPIO_Mode_IPU);

	io5.setup(GPIOB,GPIO_Pin_0,GPIO_Mode_Out_PP);
	io6.setup(GPIOB,GPIO_Pin_1,GPIO_Mode_Out_PP);
	io7.setup(GPIOB,GPIO_Pin_12,GPIO_Mode_Out_PP);
	io8.setup(GPIOB,GPIO_Pin_13,GPIO_Mode_Out_PP);

	led.setup(GPIOB,GPIO_Pin_2,GPIO_Mode_Out_PP);

	canTX.setup(GPIOA,GPIO_Pin_12,GPIO_Mode_AF_PP);
	canRX.setup(GPIOA,GPIO_Pin_11,GPIO_Mode_IN_FLOATING);

	usartTX.setup(GPIOB,GPIO_Pin_10,GPIO_Mode_AF_PP);
	usartRX.setup(GPIOB,GPIO_Pin_11,GPIO_Mode_IN_FLOATING);

	serial.setup(USART3,921600);
	serial.printf("FILE = %s\n\r",__FILE__);
	serial.printf("DATE = %s\n\r",__DATE__);
	serial.printf("TIME = %s\n\r",__TIME__);
	serial.printf("ADRS = %d\n\r",canAddress);


	CAN1Setup();

    while(1){
    	//currentA.start();
    	//delay(10);
    	serial.printf("%d\n\r",millis());



    	canData[0] = 0xFF;
    	canData[1] = 0x00;
    	//CAN1Send(CAN_ID,2,canData);
    	//while(CANTXOK != CAN_TransmitStatus(CAN1,0));


    	delay(1);
    	led.toggle();
    	io5.toggle();
    	io6.toggle();
    	io7.toggle();
    	io8.toggle();
    	/*
    	canData[0] = 0xFF;
    	canData[1] = 0xFF;
    	CAN1Send(CAN_ID,1,canData);
    	*/

    	//canData[0] = a/16;

    	//serial.printf("FIFO : %d\n\r",CAN_MessagePending(CAN1,CAN_FIFO0));
    	//CAN_GetITStatus(CAN1,CAN_IT_RQCP0);

    	while(rxFlag > 0){
    		rxFlag--;
    		serial.printf("RX FLAG CAN Data %d,%d,%d,%d,%d,%d,%d,%d\n\r",RxMessage.Data[0],RxMessage.Data[1],RxMessage.Data[2],RxMessage.Data[3],RxMessage.Data[4],RxMessage.Data[5],RxMessage.Data[6],RxMessage.Data[7]);
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

