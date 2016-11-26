#include "stm32f10x.h"
#include "stdio.h"


#include "system.h"


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

	enc1a.setup(PA0,GPIO_Mode_IPU);
	enc1b.setup(PA1,GPIO_Mode_IPU);

	enc2a.setup(PA6,GPIO_Mode_IPU);
	enc2b.setup(PA7,GPIO_Mode_IPU);

	enc3a.setup(PB8,GPIO_Mode_IPU);
	enc3b.setup(PB9,GPIO_Mode_IPU);

	enc4a.setup(PB6,GPIO_Mode_IPU);
	enc4b.setup(PB7,GPIO_Mode_IPU);

	enc1.encoderSetup(TIM1);
	enc2.encoderSetup(TIM2);
	enc3.encoderSetup(TIM3);
	enc4.encoderSetup(TIM4);

	io1.setup(PA2,GPIO_Mode_IPU);
	io2.setup(PA3,GPIO_Mode_IPU);
	io3.setup(PA4,GPIO_Mode_IPU);
	io4.setup(PA5,GPIO_Mode_IPU);

	io5.setup(PB0,GPIO_Mode_Out_PP);
	io6.setup(PB1,GPIO_Mode_Out_PP);
	io7.setup(PB12,GPIO_Mode_Out_PP);
	io8.setup(PB13,GPIO_Mode_Out_PP);

	led.setup(PB2,GPIO_Mode_Out_PP);

	canTX.setup(PA12,GPIO_Mode_AF_PP);
	canRX.setup(PA11,GPIO_Mode_IN_FLOATING);

	usartTX.setup(PB10,GPIO_Mode_AF_PP);
	usartRX.setup(PB11,GPIO_Mode_IN_FLOATING);

	serial.setup(USART3,921600);
	serial.printf("FILE = %s\n\r",__FILE__);
	serial.printf("DATE = %s\n\r",__DATE__);
	serial.printf("TIME = %s\n\r",__TIME__);

	CAN1Setup();

    while(1){
    	//serial.printf("%d\n\r",millis());

    	delay(1);
    	//led.toggle();
    	//io5.toggle();
    	//io6.toggle();
    	//io7.toggle();
    	//io8.toggle();

    	while(rxFlag > 0){
    		rxFlag--;
    		serial.printf("RX FLAG CAN Data %d,%d,%d,%d,%d,%d,%d,%d\n\r",RxMessage.Data[0],RxMessage.Data[1],RxMessage.Data[2],RxMessage.Data[3],RxMessage.Data[4],RxMessage.Data[5],RxMessage.Data[6],RxMessage.Data[7]);
    	}

    }
}




extern "C" void USB_LP_CAN1_RX0_IRQHandler(void){
	rxFlag++;
	CAN_Receive(CAN1,CAN_FIFO0,&RxMessage);
	if(RxMessage.StdId == CAN_ID){
		if(RxMessage.Data[0] & 0b00000001 != 0){
			if((RxMessage.Data[1] & 0b00000001) != 0){
				io8.write(Bit_SET);
			}else{
				io8.write(Bit_RESET);
			}
		}
		if(RxMessage.Data[0] & 0b00000010 != 0){
			if((RxMessage.Data[1] & 0b00000010) != 0){
				io7.write(Bit_SET);
			}else{
				io7.write(Bit_RESET);
			}
		}
		if(RxMessage.Data[0] & 0b00000100 != 0){
			if((RxMessage.Data[1] & 0b00000100) != 0){
				io6.write(Bit_SET);
			}else{
				io6.write(Bit_RESET);
			}
		}
		if(RxMessage.Data[0] & 0b00001000 != 0){
			if((RxMessage.Data[1] & 0b00001000) != 0){
				io5.write(Bit_SET);
			}else{
				io5.write(Bit_RESET);
			}
		}
	}

	return;
}

extern "C" void CAN1_RX1_IRQHandler(void){
	rxFlag++;
	CAN_Receive(CAN1,CAN_FIFO0,&RxMessage);
	return;
}

