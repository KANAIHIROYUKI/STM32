#include "stm32f10x.h"
#include "stdio.h"


#include "SYSTEM.h"


#define CAN_ID 0x07FF

uint16_t a;
uint16_t rxFlag = 0;

int16_t enc1=0;

uint8_t canData[8] = {0,0,0,0,0,0,0,0};

CanRxMsg RxMessage;

GPIO led;
GPIO canRx;
GPIO canTx;
GPIO usart1Tx;
GPIO usart1Rx;
GPIO usart2Tx;
GPIO usart2Rx;
GPIO aIn;
GPIO encA;
GPIO encB;

GPIO pwmout;
PWM pwm;

ADC pot;

USART serial;
//USART serial2;

int main(void)
{
	char text[] = "HOGEhoge",text2[] = "POGEpoge";
	setup();
	GPIOSetup();

	led.setup(GPIOB,GPIO_Pin_0,GPIO_Mode_Out_PP);
	canTx.setup(GPIOB,GPIO_Pin_9,GPIO_Mode_AF_PP);
	canRx.setup(GPIOB,GPIO_Pin_8,GPIO_Mode_AF_PP);

	usart1Tx.setup(GPIOA,GPIO_Pin_9,GPIO_Mode_AF_PP);
	usart1Rx.setup(GPIOA,GPIO_Pin_10,GPIO_Mode_IN_FLOATING);
	usart2Tx.setup(GPIOA,GPIO_Pin_2,GPIO_Mode_AF_PP);
	usart2Rx.setup(GPIOA,GPIO_Pin_3,GPIO_Mode_IN_FLOATING);

	aIn.setup(GPIOA,GPIO_Pin_1,GPIO_Mode_AIN);
	encA.setup(GPIOA,GPIO_Pin_0,GPIO_Mode_IPU);
	encB.setup(GPIOA,GPIO_Pin_1,GPIO_Mode_IPU);

	pwmout.setup(GPIOA,GPIO_Pin_6,GPIO_Mode_AF_PP);
	pwm.setup(TIM3,1);

	pot.setup(ADC1,9);

	serial.setup(USART1,115200);


	RCC_APB2PeriphClockCmd(RCC_APB2ENR_AFIOEN,ENABLE);

	GPIO_PinRemapConfig(GPIO_Remap1_CAN1,ENABLE);

	serial.printf("DATE = ");
	serial.printf(__DATE__);
	serial.printf("\n\rTIME = ");
	serial.printf(__TIME__);
	serial.printf("\n\r");

	serial.printf("serial.printf");

	serial.printf("\n\rADC1 CH0 read = %d\n\r",pot.read());
	CAN1Setup();

	//エンコーダ
	RCC_APB1PeriphClockCmd(RCC_APB1ENR_TIM2EN,ENABLE);
	TIM_EncoderInterfaceConfig(TIM2,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM2,ENABLE);

	//入力割込み
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
    //GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource14);
    //GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource15);

    /*
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0);

    EXTI_InitTypeDef EXTI_InitStructure;
    EXTI_StructInit(&EXTI_InitStructure);
    EXTI_InitStructure.EXTI_Line = EXTI_Line0;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);*/

    while(1){
    	pwm.duty(pot.read()/4);

    	serial.printf("%d\n\r",TIM2->CNT);
    	/*
    	serial.printf("\n\r");

    	while(serial.available()){
    		printf("rx = %d,read = %d,available = %d",USART::usart1RxAddress,USART::usart1ReadAddress,serial.available());
    		serial.send(serial.read());
    		printf("\n\r");
    		//serial.read();
    	}
    	printf("\n\r write = %d,send = %d\n\r",USART::usart1TxSendAddress,USART::usart1TxWriteAddress);
    	for(int i=0;i<USART_TX_BUFFER_SIZE;i++){
    		printf("%d,\n\r",USART::usart1TxBuffer[i]);
    	}
    	printf("\n\r");
    	*/

    	a = pot.read();
    	//OC3DutySet(TIM3,a/4);
    	//OC4DutySet(TIM3,a/4);
    	delay(2000);
    	led.write(Bit_SET);

    	delay(2000);
    	led.write(Bit_RESET);

    	canData[0] = a;
    	CAN1Send(CAN_ID,1,canData);
    	//while(CANTXOK != CAN_TransmitStatus(CAN1,0));

    	//printf("FIFO : %d\n\r",CAN_MessagePending(CAN1,CAN_FIFO0));
    	//CAN_GetITStatus(CAN1,CAN_IT_RQCP0);


    	if(rxFlag == 1){
    		rxFlag = 0;
    		//printf("CAN Data 0 : %4d ENC : %4d\n\r",RxMessage.Data[0],enc1);
    	}

    	/*if(CAN_MessagePending(CAN1,CAN_FIFO0) != 0){
    		CAN_Receive(CAN1,CAN_FIFO0,&RxMessage);
    		printf("%d\n\r",RxMessage.Data[0]);
    	}*/
    }
}







extern "C" void USB_LP_CAN1_RX0_IRQHandler(void){
	rxFlag = 1;
	CAN_Receive(CAN1,CAN_FIFO0,&RxMessage);

	return;
}

extern "C" void CAN1_RX1_IRQHandler(void){
	rxFlag = 1;
	CAN_Receive(CAN1,CAN_FIFO0,&RxMessage);
	return;
}

extern "C" void EXTI0_IRQHandler(void){
	EXTI_ClearITPendingBit(EXTI_Line0);
	enc1++;
}
