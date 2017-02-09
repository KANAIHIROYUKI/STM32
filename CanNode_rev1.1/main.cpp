
#include "main.hpp"

int main(void)
{
	setup();

	serial.printf("PIN = 0x%x,FIL = %d\n\r",canValve.canValve_address,can1.filterCnt);
	serial.printf("address = %x,%x,%x,%x",canPulse[0].canPulse_address,canPulse[1].canPulse_address,canPulse[2].canPulse_address,canPulse[3].canPulse_address);

	for(int i=0;i<can1.filterCnt;i++){
		serial.printf("%d,0x%x\n\r",can1.filterCnt,can1.filterAddress[i]);
	}

	serial.printf("boot\n\r");
	canMD[0].duty(0);
	uint64_t time = millis();
	while(time + 8000 > millis()){
		canPulse[0].cycle();
	}
	serial.printf("start\n\r");
	int16_t pulseDuty = 100;


	//IWDGSetup(PRINT_TIME * 20);
    while(1){
    	canEnc[0].cycle();
    	canPulse[0].cycle();

    	if(printTime <= millis()){
    		led.toggle();

       		printTime = millis() + PRINT_TIME;

       		//int16_t pulseDuty = millis()>>4 & 0x7FF;
       		pulseDuty +=1;
       		//pulseDuty = millis() - 1000;
       		//canVlv.write(0,millis() >> 8);

       		canMD[0].duty(pulseDuty);
       		//canMD[1].duty(pulseDuty);
       		//canMD[2].duty(pulseDuty);
       		//canMD[3].duty(pulseDuty);
       		if(pulseDuty > 700)pulseDuty = 100;



       		serial.printf("%6d,%6d,%6d,%6d\n\r",pulseDuty,canPulse[0].outDuty,(uint32_t)canPulse[0].canPulse_pulseTime,(uint32_t)canPulse[0].canPulse_cycleTime);

       		IWDGReset();

    		//serial.printf("%2d,%6d,%2d,%d\n\r",enc[0].read(),canEncoder.read(),canEncoder.lastReceiveTime(),pulseDuty);
    	}

    	while(rxFlag > 0){
    		rxFlag--;
    		//serial.printf("ADD = %x,%d\n\r",can1.rxMessage.StdId,can1.rxMessage.Data[1]);
    	}
    }
}
