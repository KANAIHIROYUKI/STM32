#include "main.hpp"

int main(void)
{
	setup();

	sys.wdgSetup(50);	//50ms
    while(1){
    	sys.cycle();
    	canMD[0].cycle();
    	canMD[1].cycle();

    	if(intervalTimer <= millis()){
    		intervalTimer = millis() + IntervalTime;
    		serial.printf("%5d,%5d,%6d,%6d\n\r",motor[0].outDuty,motor[1].outDuty,enc[0].read(),enc[1].read());

    		if(motorDebug[0] == 1){
    			if(sw[0].read() == 0){
    				canMotor[0].duty(0.2);
    			}else if(sw[1].read() == 0){
    				canMotor[0].duty(-0.2);
    			}else{
    				canMotor[0].duty(0);
    			}
    		}

    		if(motorDebug[1] == 1){
    			if(sw[0].read() == 0){
    				canMotor[1].duty(0.2);
    			}else if(sw[1].read() == 0){
    				canMotor[1].duty(-0.2);
    			}else{
    				canMotor[1].duty(0);
    			}
    		}
    	}

    }
}
