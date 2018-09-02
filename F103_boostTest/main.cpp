#include "main.hpp"

int main(void)
{
	setup();
    while(1){
    	sw[0].cycle();
    	sw[1].cycle();
    	sw[2].cycle();
    	led.cycle();


    	if(intervalTime < millis()){
    		intervalTime += 100;
    		serial.printf("%d,%d\n\r",vFB.read(),vSet.read());
    	}

    	if(controlCycleTime < millis()){
    		controlCycleTime += 10;
    		if(vFB.read() < vSet.read()){
    			pwm[0].duty(100);
    			pwm[1].duty(100);
    		}else{
    			pwm[0].duty(0);
    			pwm[1].duty(0);
    		}
    	}

    	if(sw[0].read() == 0){
    		sw[0].readStat = 0;
    		pwm[0].duty(0);
    		pwm[1].duty(0);
    		serial.printf("kick! 0");
    		kick[0].write(1);
    		delay(50);
    		kick[0].write(0);
    		delay(500);
    	}

    	if(sw[1].read() == 0){
    		sw[0].readStat = 0;
    		pwm[0].duty(0);
    		pwm[1].duty(0);
    		serial.printf("kick! 1");
    		kick[0].write(1);
    		delay(100);
    		kick[0].write(0);
    		delay(500);
    	}
    }
}
