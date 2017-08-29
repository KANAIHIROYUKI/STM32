#include "main.hpp"
#include "util.hpp"

int main(void)
{

	setup();
	sys.wdgSetup(500);

	serial.printf("%d\n\r",sys.cycleFunctionNumber);
	serial.printf("spi,%d,%d\n\r",spi.interfaceNumberCnt,mouse.spiif.interfaceNumber);

    while(1){
    	sys.cycle();
    	sa.cycle();
    	sbus.cycle();

    	if(millis() > intervalTime){
    		intervalTime += IntervalTime;

    		for(int i=0;i<4;i++){
    			serial.printf("%d,",sa.read(i));
    		}
    		serial.printf("%d,",sw.read());
    		serial.printf("\n\r");
    		//serial.printf("%d\n\r",sys.error);
    	}
    }
}
