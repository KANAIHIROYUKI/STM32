#include "main.hpp"

int main(void)
{
	setup();

	serial.printf("can_add = 0x%x\n\r",canEncoder[0].canEnc_address);

	sys.wdgSetup(50);	//50ms

    while(1){
    	sys.cycle();
    	limit[0].cycle();
    	limit[1].cycle();
    	limit[2].cycle();
    	limit[3].cycle();

    	canEnc[1].cycle();
    	canEnc[0].cycle();

    	sw[0].cycle();
    	sw[1].cycle();

    	canMD[0].cycle();
    	canMD[1].cycle();

    	canSw[0].cycle();
    	canSw[1].cycle();
    	//canSw[2].cycle();
    	//canSw[3].cycle();

    	if(intervalTimer <= millis()){
    		intervalTimer = millis() + IntervalTime;
    		//serial.printf("%5d,%5d,%6d,%6d\n\r",motor[0].outDuty,motor[1].outDuty,enc[0].read(),enc[1].read());
    		//serial.printf("%d,%d,%d,%d\n\r",(int32_t)enc[0].read(),(int32_t)enc[1].read(),(int32_t)canEncoder[0].read(),(int32_t)canEncoder[1].read());


    		serial.printf("%d,%d\n\r",canSwitch[0].read(),canSwitch[1].read());
    		//serial.printf("%d,%d,%d,%d\n\r",(uint32_t)canSwitch[0].lastReceiveTime,(uint32_t)canSwitch[1].lastReceiveTime,(uint32_t)canSwitch[2].lastReceiveTime,(uint32_t)canSwitch[3].lastReceiveTime);

    		//serial.printf("%d,%d,%d,%d\n\r",(uint32_t)canSw[0].canData[0],(uint32_t)canSw[1].canData[0],(uint32_t)canSw[2].canData[0],(uint32_t)canSw[3].canData[0]);
    		//serial.printf("%d,%d,%d,%d\n\r",(uint32_t)canSw[0].sw->read(),(uint32_t)canSw[1].sw->read(),(uint32_t)canSw[2].sw->read(),(uint32_t)canSw[3].sw->read());
    		//serial.printf("%d,%d,%d,%d\n\r",(uint32_t)canSw[0].intervalTime,(uint32_t)canSw[1].intervalTime,(uint32_t)canSw[2].intervalTime,(uint32_t)canSw[3].intervalTime);

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
