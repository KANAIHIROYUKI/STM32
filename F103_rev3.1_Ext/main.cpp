#include "stm32f10x.h"
#include "stdio.h"

#include "system.h"
#include "app.h"

#define DEBUG

#define TIM_CNT 2048
#define CAN_ADD 15 - (add[0].read()*2) - add[1].read() - (add[2].read()*8) - (add[3].read()*4)

#define CAN_MTD 0x100

uint16_t rxFlag = 0;
uint8_t canData[8] = {0,0,0,0,0,0,0,0},canAddress=0;
CanRxMsg rxMessage;

#define PRINT_TIME 50

uint64_t printTime = 0;



TIM pwm[4];
TIM enc[2];
TIM enPwm[2];
MOTOR motor[2];
CAN can1;
USART serial,vserial;
GPIO add[4],sw[2],led[2],limit[4];
ADC cs[2];

int main(void)
{
	setup();

	vserial.setup(USART1,115200,PA10,PA9);


	serial.setup(USART3,921600,PB10,PB11);
	serial.printf("\n\rFILE = %s\n\rDATE = %s\n\rTIME = %s\n\r",__FILE__,__DATE__,__TIME__);

	//serial.printf("%d,%d,%d,%d\n\r",add[0].read(),add[1].read(),add[2].read(),add[3].read());

	can1.setup(CAN1,PA12,PA11);
	can1.filterAdd(CAN_MTD,CAN_MTD+1);

	IWDGSetup(1500);

    while(1){

    	if(sw[0].read() == 0){
    		motor[0].duty(1024);
    		motor[1].duty(1024);
    	}else{
    		motor[1].duty(0);
    		motor[0].duty(0);
    	}

    	if(printTime <= millis()){
    		IWDGReset();

    		printTime = millis() + PRINT_TIME;
    		serial.printf("%d,%d\n\r",enc[0].read(),enc[1].read());
    		//serial.printf("%d,%d\n\r",cs[0].read(),cs[1].read());
    		//serial.printf("%2d,%d,%d,%d,%d\n\r",CAN_ADD,add[0].read(),add[1].read(),add[2].read(),add[3].read());
    		//serial.printf("t = %d,\n\r",millis());
    	}

    	while(rxFlag > 0){
    		rxFlag--;
    	}
    }
}

extern "C" void USB_LP_CAN1_RX0_IRQHandler(void){
	rxFlag++;
	can1.receive(&rxMessage);
	//CAN_Receive(CAN1,CAN_FIFO0,&rxMessage);

	return;
}
