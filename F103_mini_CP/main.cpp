#include "stm32f10x.h"
#include "stdio.h"
#include "SYSTEM.h"


#define IntervalTime 100

uint64_t intervalTime = 0;

GPIO led;

GPIO usartTX;
GPIO usartRX;

USART serial;

CAN can1;


int main(void)
{
	systemSetup();

	led.setup(PB0,OUTPUT);

	serial.setup(USART1,921600,PA9,PA10);
	can1.setup(CAN1,PA12,PA11);

	serial.printf("\n\rFILE = %s\n\r",__FILE__);
	serial.printf("DATE = %s\n\r",__DATE__);
	serial.printf("TIME = %s\n\r",__TIME__);

    while(1){

    	if(intervalTime < millis()){
    		led.toggle();
    		serial.printf("hoge\n\r");
    		intervalTime += IntervalTime;
    	}


    }
}


/*extern "C" void USB_LP_CAN1_RX0_IRQHandler(void){
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

*/
