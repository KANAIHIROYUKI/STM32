
#include "main.hpp"

int main(void)
{
	setup();

	sys.wdgSetup(50);	//5ms
    while(1){
    	sys.cycle();

    	canEnc[0].cycle();
    	canEnc[1].cycle();
    	canEnc[2].cycle();
    	canEnc[3].cycle();

    	if(printTime <= millis()){
    		printTime = millis() + PRINT_TIME;

    		if(canLastReceiveTime + 50 < millis()){
    			led.toggle();
    		}

        	serial.printf("itv = %2d,cnt = %8d, itv = %2d,cnt = %8d, itv = %2d,cnt = %8d, itv = %2d,cnt = %8d\n\r",(uint32_t)canEnc[0].canEnc_interval,(uint32_t)enc[0].read(),(uint32_t)canEnc[1].canEnc_interval,(uint32_t)enc[1].read(),(uint32_t)canEnc[2].canEnc_interval,(uint32_t)enc[2].read(),(uint32_t)canEnc[3].canEnc_interval,(uint32_t)enc[3].read());
   			//serial.printf("ADD = %x,0 = %d,1 = %d\n\r",can1.rxMessage.StdId,can1.rxMessage.Data[0],can1.rxMessage.Data[1]);

    	}
    }
}
