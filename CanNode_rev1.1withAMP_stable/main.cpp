
#include "main.hpp"

int main(void)
{
	setup();

	serial.printf("PIN = 0x%x,FIL = %d\n\r",canValve.canValve_address,can1.filterCnt);
	serial.printf("address = %x,%x,%x,%x",canPulse[0].canPulse_address,canPulse[1].canPulse_address,canPulse[2].canPulse_address,canPulse[3].canPulse_address);

	for(int i=0;i<can1.filterCnt;i++){
		serial.printf("%d,0x%x\n\r",can1.filterCnt,can1.filterAddress[i]);
	}

	sys.wdgSetup(50);	//5ms
    while(1){
    	sys.cycle();

    	canEnc[0].cycle();
    	canEnc[1].cycle();


    	canPulse[0].cycle();
    	canPulse[1].cycle();
    	canPulse[2].cycle();
    	canPulse[3].cycle();


    	if(printTime <= millis()){
    		printTime = millis() + PRINT_TIME;

    		if(canLastReceiveTime + 50 < millis()){
    			led.toggle();
    		}

       		serial.printf("%4d,%4d,%4d,%4d\n\r",canPulse[0].outDuty,canPulse[1].outDuty,canPulse[2].outDuty,canPulse[3].outDuty);
    	}

    	while(rxFlag > 0){
    		rxFlag--;
    		led.toggle();
    		//serial.printf("ADD = %x,%d\n\r",can1.rxMessage.StdId,can1.rxMessage.Data[1]);
    	}
    }
}
