#include "main.hpp"

int main(void)
{
	setup();
	sys.wdgSetup(500);	//50ms


	a1.start();
    while(1){
    	sys.wdgReset();
    	a0.start();
    	a1.start();

    	if(millis() > intervalTimer){
    		intervalTimer+=IntervalTime;

    		serial.printf("%d,%d\n\r",a0.peek(),a1.peek());
    	}
    }
}
