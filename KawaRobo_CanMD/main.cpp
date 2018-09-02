#include "main.hpp"

int main(void)
{
	setup();


    while(1){
    	sys.cycle();
    	sw[0].cycle();
    	sw[1].cycle();
    	for(int i=0;i<6;i++){
    		led[i].cycle();
    	}

    	if(intervalTime < millis()){
    		intervalTime += 100;
    		serial.printf("0x100 = %4d, 0x101 = %4d, 0x102 = %4d, 0x103 = %4d rxTime = %4d\n\r",(int)(motor[0].outDuty*100),(int)(motor[1].outDuty*100),(int)(motor[2].outDuty*100),(int)(motor[3].outDuty*100),(int)rx[0].lastReceiveTime);
    	}

    	for(int i=0;i<4;i++){
    		if(rx[i].isReceive()){
    			int outDuty = ((rx[i].data[1] & 0b01111111) << 8) | rx[i].data[0];
    			if((rx[i].data[1] >> 7) == 1){
    				outDuty = -outDuty;
    			}
    			float outDutyF = (float)outDuty/32767;
    			motor[i].duty(outDutyF);
    		}

    		motor[i].cycle();
    	}

    }
}
