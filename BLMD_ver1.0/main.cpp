
#include "main.hpp"

int main(void)
{
	setup();

	sys.wdgSetup(50);	//5ms
    while(1){
    	sys.cycle();
    	if(printTime <= millis()){
    		led.toggle();

       		printTime = millis() + PRINT_TIME;

       		bldc.degree(millis()/100);

       		serial.printf("%d\n\r",(uint32_t)millis());

    	}

    	while(rxFlag > 0){
    		rxFlag--;
    		//serial.printf("ADD = %x,%d\n\r",can1.rxMessage.StdId,can1.rxMessage.Data[1]);
    	}
    }
}
