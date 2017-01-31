
#include "main.hpp"

int main(void)
{
	setup();

	serial.printf("PIN = 0x%x,FIL = %d\n\r",canValve.canValve_address,can1.filterCnt);
	serial.printf("address = %x,%x,%x,%x",canPulse[0].canPulse_address,canPulse[1].canPulse_address,canPulse[2].canPulse_address,canPulse[3].canPulse_address);

	for(int i=0;i<can1.filterCnt;i++){
		serial.printf("%d,0x%x\n\r",can1.filterCnt,can1.filterAddress[i]);
	}




	IWDGSetup(PRINT_TIME * 20);
    while(1){
    	canEnc[0].cycle();

    	if(printTime <= millis()){
    		led.toggle();

       		printTime = millis() + PRINT_TIME;

       		/*
       		if(enc[0].read() > 35000){
       			pwm[0].duty(2200);
       			pwm[1].duty(2200);
       			pwm[2].duty(2200);
       			pwm[3].duty(2200);
       		}else if(enc[0].read() < -35000){
       			pwm[0].duty(800);
       			pwm[1].duty(800);
       			pwm[2].duty(800);
       			pwm[3].duty(800);
       		}else{
       			pwm[0].duty(1500 + (enc[0].read()/50));
       			pwm[1].duty(1500 + (enc[0].read()/50));
       			pwm[2].duty(1500 + (enc[0].read()/50));
       			pwm[3].duty(1500 + (enc[0].read()/50));

       		}*/


       		int16_t pulseDuty = millis()>>2 & 0x7FF;
       		pulseDuty -=1024;
       		//pulseDuty = millis() - 1000;
       		//canVlv.write(0,millis() >> 8);
       		//canMD[0].duty(pulseDuty);
       		//canMD[1].duty(pulseDuty);
       		//canMD[2].duty(pulseDuty);
       		//canMD[3].duty(pulseDuty);
       		serial.printf("%d,%d\n\r",pulseDuty,canPulse[0].outDuty);

       		IWDGReset();

    		//serial.printf("%2d,%6d,%2d,%d\n\r",enc[0].read(),canEncoder.read(),canEncoder.lastReceiveTime(),pulseDuty);
    	}

    	while(rxFlag > 0){
    		rxFlag--;
    		//serial.printf("ADD = %x,%d\n\r",can1.rxMessage.StdId,can1.rxMessage.Data[1]);
    	}
    }
}
