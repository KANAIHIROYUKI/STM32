#include "stm32f10x.h"
#include "stdio.h"

#include "system.h"
#include "app.h"
#include "pin.h"

//#define PD


#define DEBUG

#define CAN_VLV 0x280
#define CAN_MTD 0x100
#define CAN_ENC_VAL 0x440
#define CAN_ENC_SET 0x400

#define ENC_PPR 512
#define GEAR_RATIO 27
#define TIRE_DIR 20

#define PRINT_TIME 100

uint16_t rxFlag = 0,limitResetMode[4] = {3,3,3,3};

uint8_t canData[8] = {0,0,0,0,0,0,0,0},canAddress=0,serialFlag = 0;

uint64_t intervalTimer[4] = {0,0,0,0},intervalTime[4] = {0,0,0,0},printTime = 0;
uint64_t pdInterval;

CanRxMsg rxMessage;

TIM enc[4];
TIM pwm[4];
CanNodeEncoder canEnc[4];
CanNodeValve canValve;
//CAN can1;


int main(void)
{

	setup();

	enc[0].encoderSetup(TIM1,PA0,PA1);
	enc[1].encoderSetup(TIM2,PA6,PA7);
	enc[2].encoderSetup(TIM3,PB8,PB9);
	enc[3].encoderSetup(TIM4,PB6,PB7);
	enc[0].reverse();

	Can1 can1;

	canEnc[0].setup(enc[0],can1,CAN_ENC_SET);
	canEnc[1].setup(enc[1],can1,CAN_ENC_SET + 1);
	canEnc[2].setup(enc[2],can1,CAN_ENC_SET + 2);
	canEnc[3].setup(enc[3],can1,CAN_ENC_SET + 3);

	/*
	pwm[0].pwmSetup(TIM2,3,10000,72);
	pwm[1].pwmSetup(TIM2,4,10000,72);
	pwm[2].pwmSetup(TIM3,3,10000,72);
	pwm[3].pwmSetup(TIM3,4,10000,72);*/	//1MHzでカウントアップ,分解能1us単位

	Led led;
	Port0 port;
	canValve.setup(port,can1,CAN_VLV);



	Serial0 serial;
	serial.printf("\n\rFILE = %s\n\rDATE = %s\n\rTIME = %s\n\r",__FILE__,__DATE__,__TIME__);
	serial.printf("%d\n\r",port.port_num);


	//can1.setup();
	//can1.filterAdd(0x100,0x101,0x102,0x103);
	//can1.filterAdd(0x104,0x105,0x106,0x107);
	can1.filterAdd(CAN_VLV);
	/*
	can1.filterAdd(CAN_ENC_SET,CAN_ENC_SET + 1,CAN_ENC_SET + 2,CAN_ENC_SET + 3);
	can1.filterAdd(CAN_ENC_VAL,CAN_ENC_VAL + 1,CAN_ENC_VAL + 2,CAN_ENC_VAL + 3);*/


#ifdef DEBUG
	canEnc[0].canEnc_interval = 100;
	intervalTime[0] = 100;
#endif

	uint16_t pwmCnt = 0;

	//IWDGSetup(1500);

    while(1){
    	if(pwmCnt > 2000){
    		pwmCnt = 1000;
    	}

    	for(int i=0;i<4;i++){
    		canEnc[i].cycle();
    		/*
        	if(intervalTime[i] != 0){
            	if(intervalTimer[i] < millis()){
            		uint32_t encValue = enc[i].read();
            		intervalTimer[i] = millis() + intervalTime[i];

            		canData[0] = encValue & 0xFF;
            		canData[1] = (encValue >> 8) & 0xFF;
            		canData[2] = (encValue >> 16) & 0xFF;
            		canData[3] = (encValue >> 24) & 0xFF;
            		CAN1Send(CAN_ENC_VAL + i,4,canData);
            	}
        	}*/
    	}


    	if(printTime < millis()){
    		led.toggle();
    		port.invert();

    		IWDGReset();

    		//pwmCnt+=10;
    		//pwm[0].duty(pwmCnt);

    		printTime = millis() + PRINT_TIME;
    		//serial.printf("itv = %2d,cnt = %8d, itv = %2d,cnt = %8d, itv = %2d,cnt = %8d, itv = %2d,cnt = %8d\n\r",(uint32_t)intervalTime[0],(uint32_t)enc[0].read(),(uint32_t)intervalTime[1],(uint32_t)enc[1].read(),(uint32_t)intervalTime[2],(uint32_t)enc[2].read(),(uint32_t)intervalTime[3],(uint32_t)enc[3].read());
    		serial.printf("itv = %2d,cnt = %8d, itv = %2d,cnt = %8d\n\r",(uint32_t)canEnc[0].canEnc_interval,(uint32_t)canEnc[0].canEnc_enc->read(),(uint32_t)canEnc[1].canEnc_interval,(uint32_t)canEnc[1].canEnc_enc->read());
    	}

    	while(rxFlag > 0){
    		rxFlag--;

    		//serial.printf("intTime = %d,%d,%d,%d\n\r",intervalTime[0],intervalTime[1],intervalTime[2],intervalTime[3]);
    		//serial.printf("CAN ID = 0x,%x,DATA = ,%d,%d,%d,%d,%d,%d,%d,%d\n\r",rxMessage.StdId,rxMessage.Data[0],rxMessage.Data[1],rxMessage.Data[2],rxMessage.Data[3],rxMessage.Data[4],rxMessage.Data[5],rxMessage.Data[6],rxMessage.Data[7]);
    	}
    }
}

extern "C" void USB_LP_CAN1_RX0_IRQHandler(void){
	Port0 port;
	rxFlag++;
	//can1.receive(&rxMessage);
	CAN_Receive(CAN1,CAN_FIFO0,&rxMessage);

	canValve.interrupt(rxMessage);
	for(int i=0;i<4;i++){
		canEnc[i].interrupt(rxMessage);
	}

	if(rxMessage.StdId == CAN_VLV){
		for(int i=0;i<8;i++){
			if((rxMessage.Data[0] & (1 << i)) != 0){
				if(((rxMessage.Data[1] & (1 << i)) != 0)){
					port.io[i].write(Bit_SET);
				}else{
					port.io[i].write(Bit_RESET);
				}
			}
		}
	}

/*
	if(rxMessage.StdId == CAN_VLV){
		for(int i=0;i<8;i++){
			if((rxMessage.Data[0] & (1 << i)) != 0){
				if(((rxMessage.Data[1] & (1 << i)) != 0)){
					port.io[i].write(Bit_SET);
				}else{
					port.io[i].write(Bit_RESET);
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
	}*/
	//serial.printf("CAN ID = 0x,%x,DATA = ,%d,%d,%d,%d,%d,%d,%d,%d\n\r",rxMessage.StdId,rxMessage.Data[0],rxMessage.Data[1],rxMessage.Data[2],rxMessage.Data[3],rxMessage.Data[4],rxMessage.Data[5],rxMessage.Data[6],rxMessage.Data[7]);
	//serial.printf("%x\n\r",rxMessage.StdId);
	return;
}
