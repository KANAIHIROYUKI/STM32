#include "stm32f10x.h"
#include "stdio.h"

#include "system.h"
#include "app.h"

#define CAN_VLV 0x280
#define CAN_MTD 0x100
#define CAN_ENC_VAL 0x440
#define CAN_ENC_SET 0x400

uint16_t rxFlag = 0,limitResetMode[4] = {3,3,3,3};

float currentA_A=0,currentB_A=0;

uint8_t canData[8] = {0,0,0,0,0,0,0,0},canAddress=0;

uint64_t intervalTimer[4] = {0,0,0,0},intervalTime[4] = {0,0,0,0};

CanRxMsg rxMessage;

GPIO enc1a;
GPIO enc1b;
GPIO enc2a;
GPIO enc2b;
GPIO enc3a;
GPIO enc3b;
GPIO enc4a;
GPIO enc4b;

GPIO io[8];

GPIO led;

GPIO usartTX;
GPIO usartRX;
GPIO canTX;
GPIO canRX;

TIM enc[4];

USART serial;

CAN can1;

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

	enc[0].encoderSetup(TIM1);
	enc[1].encoderSetup(TIM2);
	enc[2].encoderSetup(TIM3);
	enc[3].encoderSetup(TIM4);

	io[0].setup(PA2,GPIO_Mode_AF_PP);
	io[1].setup(PA3,GPIO_Mode_AF_PP);
	io[2].setup(PA4,GPIO_Mode_AF_PP);
	io[3].setup(PA5,GPIO_Mode_AF_PP);

	io[4].setup(PB0,GPIO_Mode_AF_PP);
	io[5].setup(PB1,GPIO_Mode_AF_PP);
	io[6].setup(PB12,GPIO_Mode_AF_PP);
	io[7].setup(PB13,GPIO_Mode_AF_PP);

	led.setup(PB2,GPIO_Mode_Out_PP);

	canTX.setup(PA12,GPIO_Mode_AF_PP);
	canRX.setup(PA11,GPIO_Mode_IN_FLOATING);

	usartTX.setup(PB10,GPIO_Mode_AF_PP);
	usartRX.setup(PB11,GPIO_Mode_IN_FLOATING);

	serial.setup(USART3,921600);
	serial.printf("FILE = %s\n\r",__FILE__);
	serial.printf("DATE = %s\n\r",__DATE__);
	serial.printf("TIME = %s\n\r",__TIME__);


	can1.setup();
	can1.filterAdd(CAN_VLV);
	can1.filterAdd(CAN_ENC_SET,CAN_ENC_SET + 1,CAN_ENC_SET + 2,CAN_ENC_SET + 3);
	can1.filterAdd(CAN_ENC_VAL,CAN_ENC_VAL + 1,CAN_ENC_VAL + 2,CAN_ENC_VAL + 3);

	//CAN1Setup();
	//CANFilterAdd(CAN_VLV);
	//CANFilterAdd(CAN_ENC_SET,CAN_ENC_SET + 1,CAN_ENC_SET + 2,CAN_ENC_SET + 3);
	//CANFilterAdd(CAN_ENC_VAL,CAN_ENC_VAL + 1,CAN_ENC_VAL + 2,CAN_ENC_VAL + 3);

    while(1){
    	for(int i=0;i<4;i++){
        	if(intervalTime[i] != 0){
            	if(intervalTimer[i] < millis()){
            		uint32_t encValue = enc[i].read();
            		intervalTimer[i] = millis() + intervalTime[i];
            		canData[0] = encValue & 0xFF;
            		canData[1] = (encValue >> 8) & 0xFF;
            		canData[2] = (encValue >> 16) & 0xFF;
            		canData[3] = (encValue >> 24) & 0xFF;
            		CAN1Send(CAN_ENC_VAL + i,4,canData);
            		serial.printf("%d,%d,%d,%d\n\r",enc[0].read(),enc[1].read(),enc[2].read(),enc[3].read());
            	}
        	}
    	}

    	while(rxFlag > 0){
    		rxFlag--;
    		serial.printf("RX FLAG CAN Data 0x%x,%d,%d,%d,%d,%d,%d,%d,%d\n\r",rxMessage.StdId,rxMessage.Data[0],rxMessage.Data[1],rxMessage.Data[2],rxMessage.Data[3],rxMessage.Data[4],rxMessage.Data[5],rxMessage.Data[6],rxMessage.Data[7]);
    	}
    }
}

extern "C" void USB_LP_CAN1_RX0_IRQHandler(void){
	rxFlag++;
	//CAN_Receive(CAN1,CAN_FIFO0,&rxMessage);
	can1.receive(&rxMessage);
	if(rxMessage.StdId == CAN_VLV){
		for(int i=0;i<8;i++){
			if((rxMessage.Data[0] & (1 << i)) != 0){
				if(((rxMessage.Data[1] & (1 << i)) != 0)){
					io[i].write(Bit_SET);
				}else{
					io[i].write(Bit_RESET);
				}
			}
		}
	}else if((rxMessage.StdId >= CAN_ENC_SET) && (rxMessage.StdId <= CAN_ENC_SET + 3)){
		for(int i=0;i<4;i++){
			if(rxMessage.Data[0] == 0){
				enc[i].reset();
			}else if(rxMessage.Data[0] == 1){
				intervalTime[i] = (rxMessage.Data[2] << 8) + rxMessage.Data[1];
			}
		}
	}/*else if(rxMessage.StdId == CAN_ENC_SET){
		if(rxMessage.Data[0] == 0){
			encValue[0] = 0;
		}else if(rxMessage.Data[0] == 1){
			intervalTime[0] = (rxMessage.Data[2] << 8) + rxMessage.Data[1];
		}
	}else if(rxMessage.StdId == CAN_ENC_SET + 1){
		if(rxMessage.Data[0] == 0){
			encValue[1] = 0;
		}else if(rxMessage.Data[0] == 1){
			intervalTime[1] = (rxMessage.Data[2] << 8) + rxMessage.Data[1];
		}
	}else if(rxMessage.StdId == CAN_ENC_SET + 2){
		if(rxMessage.Data[0] == 0){
			encValue[2] = 0;
		}else if(rxMessage.Data[0] == 1){
			intervalTime[2] = (rxMessage.Data[2] << 8) + rxMessage.Data[1];
		}
	}else if(rxMessage.StdId == CAN_ENC_SET + 3){
		if(rxMessage.Data[0] == 0){
			encValue[3] = 0;
		}else if(rxMessage.Data[0] == 1){
			intervalTime[3] = (rxMessage.Data[2] << 8) + rxMessage.Data[1];
		}
	}*/

	return;
}

