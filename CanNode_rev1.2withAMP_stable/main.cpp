
#include "main.hpp"

int main(void)
{
	setup();
	serial.printf("enc setup address = %x,%x\n\r",canEnc[0].canEnc_address,canEnc[1].canEnc_address);
	serial.printf("enc valur address = %x,%x\n\r",canEnc[0].canEnc_address + 0x40,canEnc[1].canEnc_address + 0x40);
	serial.printf("vlv address =  %x,filter number = %d\n\r",canValve.canValve_address,can1.filterCnt);
	serial.printf("pls address = %x,%x,%x,%x",canPulse[0].canPulse_address,canPulse[1].canPulse_address,canPulse[2].canPulse_address,canPulse[3].canPulse_address);

	for(int i=0;i<can1.filterCnt;i++){
		serial.printf("%d,0x%x\n\r",can1.filterCnt,can1.filterAddress[i]);
	}

	//serial.printf("int = %d,%d\n\r",canEnc[0].canEnc_interval,canEnc[1].canEnc_interval);

	sys.wdgSetup(50);	//5ms
    while(1){
    	sys.cycle();

    	if(sw.read() == 0){
    		if(switchStat == 0){
    			switchStat = 1;
    			printMode++;
    		}
    	}else{
    		switchStat = 0;
    	}

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

    		switch(printMode){
       		case 0:
       			serial.printf("%4d,%4d\n\r",canPulse[0].outDuty,canPulse[1].outDuty);
       			break;
       		case 1:
       			serial.printf("%4d,%4d,%4d,%4d\n\r",canEnc[0].canEnc_interval,canEnc[0].canEnc_enc->read(),canEnc[1].canEnc_interval,canEnc[1].canEnc_enc->read());
       			break;
       		case 2:

       			break;
       		default:

       			break;
       		}
    	}

    	while(rxFlag > 0){
    		rxFlag--;
    		led.toggle();
    		//serial.printf("ADD = %x,%d\n\r",can1.rxMessage.StdId,can1.rxMessage.Data[1]);
    	}
    }
}
