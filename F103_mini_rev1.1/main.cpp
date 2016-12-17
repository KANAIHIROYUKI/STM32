#include "stm32f10x.h"
#include "stdio.h"

#include "system.h"
#include "app.h"

//#define PD

//#define DEBUG

#define CAN_VLV 0x280
#define CAN_MTD 0x100
#define CAN_ENC_VAL 0x440
#define CAN_ENC_SET 0x400

#define ENC_PPR 512
#define GEAR_RATIO 27
#define TIRE_DIR 20

uint16_t rxFlag = 0,limitResetMode[4] = {3,3,3,3};

float currentA_A=0,currentB_A=0;

uint8_t canData[8] = {0,0,0,0,0,0,0,0},canAddress=0;

uint64_t intervalTimer[4] = {0,0,0,0},intervalTime[4] = {0,0,0,0};
uint64_t pdInterval;

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

#ifdef PD
TIM pwm[4];
#endif

USART serial;

CAN can1;

int main(void)
{
	setup();

	enc1a.setup(PA0,INPUT_PU);
	enc1b.setup(PA1,INPUT_PU);

	enc2a.setup(PA6,INPUT_PU);
	enc2b.setup(PA7,INPUT_PU);

	enc3a.setup(PB8,INPUT_PU);
	enc3b.setup(PB9,INPUT_PU);

	enc4a.setup(PB6,INPUT_PU);
	enc4b.setup(PB7,INPUT_PU);

	enc[0].encoderSetup(TIM1);
#ifndef PD
	enc[1].encoderSetup(TIM2);
	enc[2].encoderSetup(TIM3);
#endif
	enc[3].encoderSetup(TIM4);

#ifdef PD
	pwm[0].pwmSetup(TIM2,3);
	pwm[1].pwmSetup(TIM2,4);
	pwm[2].pwmSetup(TIM3,3);
	pwm[3].pwmSetup(TIM3,4);
#endif

#ifndef PD
	io[0].setup(PA2,OUTPUT);
	io[1].setup(PA3,OUTPUT);
	io[2].setup(PA4,OUTPUT);
	io[3].setup(PA5,OUTPUT);

	io[4].setup(PB0,OUTPUT);
	io[5].setup(PB1,OUTPUT);
	io[6].setup(PB12,OUTPUT);
	io[7].setup(PB13,OUTPUT);
#endif

#ifdef PD
	io[0].setup(PA2,OUTPUT_AF);
	io[1].setup(PA3,OUTPUT_AF);
	io[2].setup(PA4,OUTPUT);
	io[3].setup(PA5,OUTPUT);

	io[4].setup(PB0,OUTPUT_AF);
	io[5].setup(PB1,OUTPUT_AF);
	io[6].setup(PB12,OUTPUT);
	io[7].setup(PB13,OUTPUT);
#endif

	led.setup(PB2,OUTPUT);

	canTX.setup(PA12,OUTPUT_AF);
	canRX.setup(PA11,INPUT);

	usartTX.setup(PB10,OUTPUT_AF);
	usartRX.setup(PB11,INPUT);

	serial.setup(USART3,921600);
	serial.printf("FILE = %s\n\r",__FILE__);
	serial.printf("DATE = %s\n\r",__DATE__);
	serial.printf("TIME = %s\n\r",__TIME__);


	can1.setup();
	can1.filterAdd(CAN_VLV);
	can1.filterAdd(CAN_ENC_SET,CAN_ENC_SET + 1,CAN_ENC_SET + 2,CAN_ENC_SET + 3);
	can1.filterAdd(CAN_ENC_VAL,CAN_ENC_VAL + 1,CAN_ENC_VAL + 2,CAN_ENC_VAL + 3);

	enc[0].reverse();

#ifdef DEBUG
	intervalTime[0] = 100;
#endif

#ifdef PD
	pdInterval = millis() + 1000;
#endif

    while(1){
#ifdef PD
    	if(pdInterval < millis()){
    		pdInterval = millis() + 10000;
    		pwm[0].duty(1024);
    		enc[0].reset();
    		while(enc[0].read() < ENC_PPR * GEAR_RATIO * 10){
    			delay(10);
    			serial.printf("%d\n\r",enc[0].read());
    		}
    		pwm[0].duty(0);
    	}
#endif

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
	}else if(rxMessage.StdId == CAN_ENC_SET){
		if(rxMessage.Data[0] == 0){
			enc[0].reset();
		}else if(rxMessage.Data[0] == 1){
			intervalTime[0] = (rxMessage.Data[2] << 8) + rxMessage.Data[1];
		}
	}else if(rxMessage.StdId == CAN_ENC_SET + 1){
		if(rxMessage.Data[0] == 0){
			enc[1].reset();
		}else if(rxMessage.Data[0] == 1){
			intervalTime[1] = (rxMessage.Data[2] << 8) + rxMessage.Data[1];
		}
	}else if(rxMessage.StdId == CAN_ENC_SET + 2){
		if(rxMessage.Data[0] == 0){
			enc[2].reset();
		}else if(rxMessage.Data[0] == 1){
			intervalTime[2] = (rxMessage.Data[2] << 8) + rxMessage.Data[1];
		}
	}else if(rxMessage.StdId == CAN_ENC_SET + 3){
		if(rxMessage.Data[0] == 0){
			enc[3].reset();
		}else if(rxMessage.Data[0] == 1){
			intervalTime[3] = (rxMessage.Data[2] << 8) + rxMessage.Data[1];
		}
	}

	return;
}

