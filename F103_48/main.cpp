#include "stm32f10x.h"
#include "STM32F103C8T6_Function.h"
#include "stdio.h"

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
GPIO usart2Tx;
GPIO aIn;
GPIO encA;
GPIO encB;
GPIO encZ;

int main(void)
{
	GPIOSetup();

	led.setup(GPIOB,GPIO_Pin_0,GPIO_Mode_Out_PP);
	canTx.setup(GPIOB,GPIO_Pin_9,GPIO_Mode_AF_PP);
	canRx.setup(GPIOB,GPIO_Pin_8,GPIO_Mode_AF_PP);
	usart1Tx.setup(GPIOA,GPIO_Pin_9,GPIO_Mode_AF_PP);
	usart2Tx.setup(GPIOA,GPIO_Pin_2,GPIO_Mode_AF_PP);
	aIn.setup(GPIOA,GPIO_Pin_1,GPIO_Mode_AIN);
	encA.setup(GPIOA,GPIO_Pin_14,GPIO_Mode_IN_FLOATING);
	encB.setup(GPIOA,GPIO_Pin_15,GPIO_Mode_IN_FLOATING);
	encZ.setup(GPIOA,GPIO_Pin_0,GPIO_Mode_IN_FLOATING);



	RCC_APB2PeriphClockCmd(RCC_APB2ENR_AFIOEN,ENABLE);

	GPIO_PinRemapConfig(GPIO_Remap1_CAN1,ENABLE);

	SysTick_Config(SystemCoreClock/1000);

	USART2Setup(9600);
	USART2ITSetup(USART_IT_RXNE);

	USART1Setup(9600);
	USART1ITSetup(USART_IT_RXNE);

/*	TIM3PWMSetup(1024);
	OC3PWMSetup(TIM3,TIM_OCMode_PWM1);
	OC4PWMSetup(TIM3,TIM_OCMode_PWM1);

	OC3DutySet(TIM3,512);
	OC4DutySet(TIM3,512);*/

	ADC1Setup(1);

	CAN1Setup();

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
    //GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource14);
    //GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource15);
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
    NVIC_Init(&NVIC_InitStructure);

    while(1){
    	a = ADC1Read(1);
    	//OC3DutySet(TIM3,a/4);
    	//OC4DutySet(TIM3,a/4);
    	delay(200);
    	led.write(Bit_SET);

    	delay(200);
    	led.write(Bit_RESET);

    	canData[0] = a;
    	CAN1Send(CAN_ID,1,canData);
    	//while(CANTXOK != CAN_TransmitStatus(CAN1,0));

    	//printf("FIFO : %d\n\r",CAN_MessagePending(CAN1,CAN_FIFO0));
    	//CAN_GetITStatus(CAN1,CAN_IT_RQCP0);


    	if(rxFlag == 1){
    		rxFlag = 0;
    		printf("CAN Data 0 : %4d ENC : %4d\n\r",RxMessage.Data[0],enc1);
    	}

    	/*if(CAN_MessagePending(CAN1,CAN_FIFO0) != 0){
    		CAN_Receive(CAN1,CAN_FIFO0,&RxMessage);
    		printf("%d\n\r",RxMessage.Data[0]);
    	}*/
    }
}





extern "C" void USART2_IRQHandler(void){
	if(USART_GetITStatus(USART2,USART_IT_RXNE) == SET){
		uint8_t i = USART_ReceiveData(USART2);
		USART_SendData(USART2,i);

		//usartData[usartAddress] = USART_ReceiveData(USART2);
		//usartAddress++;
	}
}

extern "C" void USART1_IRQHandler(void){
	if(USART_GetITStatus(USART1,USART_IT_RXNE) == SET){
		uint8_t i = USART_ReceiveData(USART1);
		USART_SendData(USART1,i);

		//usartData[usartAddress] = USART_ReceiveData(USART2);
		//usartAddress++;
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


