
#include "main.hpp"

int main(void)
{
	setup();

	IWDGSetup(PRINT_TIME * 20);
    while(1){
    	canEnc[0].cycle();
    	canEnc[1].cycle();
    	canEnc[2].cycle();
    	canEnc[3].cycle();

    	if(sw.read() == 0){
    		if(switchStat == 0){
    			switchStat = 1;
    			printMode++;
    		}
    	}else{
    		switchStat = 0;
    	}

    	if(printTime <= millis()){
    		led.toggle();
       		printTime = millis() + PRINT_TIME;

       		IWDGReset();
       		switch(printMode){
       		case 0:
       			serial.printf("itv = %2d,cnt = %8d, itv = %2d,cnt = %8d, itv = %2d,cnt = %8d, itv = %2d,cnt = %8d\n\r",(uint32_t)canEnc[0].canEnc_interval,(uint32_t)enc[0].read(),(uint32_t)canEnc[1].canEnc_interval,(uint32_t)enc[1].read(),(uint32_t)canEnc[2].canEnc_interval,(uint32_t)enc[2].read(),(uint32_t)canEnc[3].canEnc_interval,(uint32_t)enc[3].read());
       			break;
       		case 1:
       			serial.printf("ADD = %x,0 = %d,1 = %d\n\r",can1.rxMessage.StdId,can1.rxMessage.Data[0],can1.rxMessage.Data[1]);
       			break;
       		case 2:
       			//serial.printf("0x%x\n\r",);
       			break;
       		case 3:

       			break;
       		case 4:

       			break;
       		default:
       			printMode = 0;
       			break;
       		}

    	}

    	while(rxFlag > 0){
    		rxFlag--;
    		//serial.printf("ADD = %x,0 = %d,1 = %d\n\r",can1.rxMessage.StdId,can1.rxMessage.Data[0],can1.rxMessage.Data[1]);
    	}
    }
}
