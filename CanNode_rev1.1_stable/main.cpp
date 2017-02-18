
#include "main.hpp"

int main(void)
{
	setup();
	serial.printf("0x%x\n\r",canEnc[0].canEnc_address);

	IWDGSetup(PRINT_TIME * 20);
    while(1){
    	canEnc[0].cycle();
    	canEnc[1].cycle();
    	canEnc[2].cycle();
    	canEnc[3].cycle();

    	if(printTime <= millis()){
    		led.toggle();

       		printTime = millis() + PRINT_TIME;

       		IWDGReset();
    		serial.printf("itv = %2d,cnt = %8d, itv = %2d,cnt = %8d, itv = %2d,cnt = %8d, itv = %2d,cnt = %8d\n\r",(uint32_t)canEnc[0].canEnc_interval,(uint32_t)enc[0].read(),(uint32_t)canEnc[1].canEnc_interval,(uint32_t)enc[1].read(),(uint32_t)canEnc[2].canEnc_interval,(uint32_t)enc[2].read(),(uint32_t)canEnc[3].canEnc_interval,(uint32_t)enc[3].read());

    		//serial.printf("%2d,%6d,%2d,%d\n\r",enc[0].read(),canEncoder.read(),canEncoder.lastReceiveTime(),pulseDuty);
    	}

    	while(rxFlag > 0){
    		rxFlag--;
    		serial.printf("ADD = %x,0 = %d,1 = %d\n\r",can1.rxMessage.StdId,can1.rxMessage.Data[0],can1.rxMessage.Data[1]);
    	}
    }
}
