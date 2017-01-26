#include "main.hpp"

int main(void)
{
	setup();

	serial.printf("INT = %d\n\r",canEnc[0].canEnc_interval);

	IWDGSetup(PRINT_TIME * 20);

    while(1){
    	canEnc[0].cycle();

    	if(printTime <= millis()){
       		printTime = millis() + PRINT_TIME;
    		IWDGReset();

    		serial.printf("%d,%d,%d\n\r",enc[0].read(),canEncoder.canEnc_value,canEnc[0].canEnc_intervalTimer);
    	}

    	while(rxFlag > 0){
    		rxFlag--;
    		//serial.printf("ADD = %x,%d\n\r",can1.rxMessage.StdId,can1.rxMessage.Data[1]);
    	}
    }
}
