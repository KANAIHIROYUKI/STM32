
#include "main.hpp"

int main(void)
{
	setup();

	IWDGSetup(PRINT_TIME * 20);
    while(1){

    	if(printTime <= millis()){
    		led.toggle();

       		printTime = millis() + PRINT_TIME;

       		IWDGReset();

    		//serial.printf("%2d,%6d,%2d,%d\n\r",enc[0].read(),canEncoder.read(),canEncoder.lastReceiveTime(),pulseDuty);
    	}

    	while(rxFlag > 0){
    		rxFlag--;
    		//serial.printf("ADD = %x,%d\n\r",can1.rxMessage.StdId,can1.rxMessage.Data[1]);
    	}
    }
}
