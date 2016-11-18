#include "stm32f10x.h"
#include "stdio.h"


#include "SYSTEM.h"


#define CAN_ID 0x100

#define IntervalTime 100

uint16_t a,adcChannelStat = 1;
uint16_t rxFlag = 0,tim4itCnt=0,currentA_16,currentB_16;

float currentA_A=0,currentB_A=0;

uint8_t canData[8] = {0,0,0,0,0,0,0,0},canAddress=0;

uint64_t intervalTimer = 0;

CanRxMsg RxMessage;

GPIO encAa;
GPIO encAb;
GPIO encBa;
GPIO encBb;

GPIO pinCurrentA;
GPIO pinCurrentB;

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

TIM pwmA;
TIM pwmAN;
TIM pwmB;
TIM pwmBN;

TIM encA;
TIM encB;

ADC currentA;
ADC currentB;

USART serial;



int main(void)
{
	setup();
	GPIOSetup();

	encAa.setup(GPIOA,GPIO_Pin_0,GPIO_Mode_IPU);
	encAb.setup(GPIOA,GPIO_Pin_1,GPIO_Mode_IPU);
	encBa.setup(GPIOA,GPIO_Pin_0,GPIO_Mode_IPU);
	encBb.setup(GPIOA,GPIO_Pin_1,GPIO_Mode_IPU);
	encA.encoderSetup(TIM2);
	encB.encoderSetup(TIM3);

	pinCurrentA.setup(GPIOA,GPIO_Pin_2,GPIO_Mode_AIN);
	pinCurrentB.setup(GPIOA,GPIO_Pin_3,GPIO_Mode_AIN);
	currentA.setup(ADC1,2);
	currentB.setup(ADC1,3);

	ledA.setup(GPIOB,GPIO_Pin_0,GPIO_Mode_Out_PP);
	ledB.setup(GPIOA,GPIO_Pin_5,GPIO_Mode_Out_PP);
	buzzer.setup(GPIOB,GPIO_Pin_0,GPIO_Mode_Out_PP);

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
	canRX.setup(GPIOA,GPIO_Pin_11,GPIO_Mode_IN_FLOATING);

	spiNSS.setup(GPIOA,GPIO_Pin_15,GPIO_Mode_IPU);
	spiSCK.setup(GPIOB,GPIO_Pin_3,GPIO_Mode_AF_PP);
	spiMOSI.setup(GPIOB,GPIO_Pin_4,GPIO_Mode_AF_PP);
	spiMISO.setup(GPIOB,GPIO_Pin_5,GPIO_Mode_AF_PP);

	pinPwmA.setup(GPIOB,GPIO_Pin_8,GPIO_Mode_AF_PP);
	pinPwmAN.setup(GPIOB,GPIO_Pin_9,GPIO_Mode_AF_PP);
	pinPwmB.setup(GPIOB,GPIO_Pin_6,GPIO_Mode_AF_PP);
	pinPwmBN.setup(GPIOB,GPIO_Pin_7,GPIO_Mode_AF_PP);


	pwmA.pwmSetup(TIM4,3,1024,TIM_OCMode_PWM1);
	pwmAN.pwmSetup(TIM4,4,1024,TIM_OCMode_PWM1);
	pwmB.pwmSetup(TIM4,1,1024,TIM_OCMode_PWM1);
	pwmBN.pwmSetup(TIM4,2,1024,TIM_OCMode_PWM1);
	pwmA.duty(512);
	pwmAN.duty(512);
	pwmB.duty(512);
	pwmBN.duty(512);
	TIM4ITSetup();


	motorEN.write(Bit_SET);

	//SPI1Setup(SPI_Mode_Master,SPI_Mode0,SPI_BaudRatePrescaler_256);
	//spiNss.write(Bit_SET);
	//SPI_I2S_SendData(SPI1,0b01010101);

	serial.setup(USART1,921600);
	serial.printf("FILE = %s\n\r",__FILE__);
	serial.printf("DATE = %s\n\r",__DATE__);
	serial.printf("TIME = %s\n\r",__TIME__);
	serial.printf("ADRS = %d\n\r",canAddress);


	CAN1Setup();

	/*
	buzzer.write(Bit_SET);
	delay(500);
	buzzer.write(Bit_RESET);*/

	//CANエンコーダ
/*
	canData[0] = 0xFF;
	canData[1] = 0x00;
	CAN1Send(CAN_ID,8,canData);*/

	/*
	RCC_APB1PeriphClockCmd(RCC_APB1ENR_TIM2EN,ENABLE);
	TIM_EncoderInterfaceConfig(TIM2,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM2,ENABLE);*/

	//入力割込み
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
    	//currentA.start();
    	//delay(10);
    	serial.printf("%d\n\r",millis());



    	canData[0] = 0xFF;
    	canData[1] = 0x00;
    	CAN1Send(CAN_ID,2,canData);
    	//while(CANTXOK != CAN_TransmitStatus(CAN1,0));


    	delay(100);
    	ledA.toggle();
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

/*
extern "C" void EXTI0_IRQHandler(void){
	EXTI_ClearITPendingBit(EXTI_Line0);
}*/



extern "C" void TIM4_IRQHandler(void){
	if(adcChannelStat == 1){
		adcChannelStat = 2;
		currentB_16 = currentB.peek();
		//currentB_A = (currentB.peek() - 1024)*0.01953125;
		currentA.start(ADC_SampleTime_239Cycles5);
	}else{
		adcChannelStat = 1;
		currentA_16 = currentA.peek();
		//currentA_A = (currentA.peek() - 1024)*0.01953125;
		currentB.start(ADC_SampleTime_239Cycles5);
	}
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);

}


