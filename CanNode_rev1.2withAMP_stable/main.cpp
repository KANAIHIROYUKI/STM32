
#include "main.hpp"

int main(void)
{
	setup();


	//serial.printf("int = %d,%d\n\r",canEnc[0].canEnc_interval,canEnc[1].canEnc_interval);

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
    		//serial.printf("%d,%d\n\r",canEnc[0].canEnc_interval,canEnc[1].canEnc_interval);

#ifdef DEBUG
    		serial.printf("%d,%d\n\r",(int32_t)canEncoder.read(),(int32_t)canEncoder.read());
#endif

        	if(sw.read() == 0){
        		if(switchStat == 0){
        			switchStat = 1;
        			printMode++;
        		}
        	}else{
        		switchStat = 0;
        	}

    		switch(printMode){
       		case 0:
       			serial.printf("duty=%4d,%4d\n\r",canPulse[0].outDuty,canPulse[1].outDuty);
       			break;
       		case 1:
       			serial.printf("itv=%4d,cnt=%6d,itv=%4d,cnt=%4d\n\r",canEnc[0].canEnc_interval,(int32_t)canEnc[0].canEnc_enc->read(),canEnc[1].canEnc_interval,(int32_t)canEnc[1].canEnc_enc->read());
       			break;
       		case 2:
       			//CANŽóM
       			break;
       		default:
       			printMode = 0;
       			break;
       		}
    	}

    	while(rxFlag > 0){
    		rxFlag--;
    		led.toggle();
    		if(printMode == 2)serial.printf("ADD = %x,%d\n\r",can1.rxMessage.StdId,can1.rxMessage.Data[1]);
    	}
    }
}
