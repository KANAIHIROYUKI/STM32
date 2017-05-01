#include "main.hpp"

int main(void)
{
	setup();

	serial.printf("pinnum = 0x%x\n\r",canSw.switchNumber);

	//sys.wdgSetup(500);	//50ms


    while(1){
    	for(int i=0;i<4;i++){
    		limit[i].cycle();
    	}

    	sys.cycle();

    	sw[0].cycle();
    	sw[1].cycle();

    	canEnc[1].cycle();
    	canEnc[0].cycle();

    	canSw.cycle();

    	canVol.cycle();

    	canMD[0].cycle();
    	canMD[1].cycle();

    	driver.cycle();


    	if(intervalTimer <= millis()){

    		intervalTimer = millis() + IntervalTime;

    		//serial.printf("%d\n\r",CAN_ADDRESS);
    		//serial.printf("%d,%d,%d,%d\n\r",limit[0].gpioRead(),limit[1].gpioRead(),limit[2].gpioRead(),limit[3].gpioRead());
    		//serial.printf("%d,%d,%d\n\r",isoIn.read(0),isoIn.read(1),isoIn.read(2));
    		////serial.printf("%d,%d,%d,%d,%d\n\r",(uint32_t)driver.vbattRead(),(uint32_t)driver.currentRread(0),(uint32_t)driver.currentRread(1),driver.currentValue[0],driver.currentValue[1]);
    		serial.printf("vval=%d,stat=%d,error=0x%x,en=%d,v0=%d,v1=%d,v2=%d\n\r",canVoltage.read(),driver.driveStat,driver.driveError,(uint32_t)driver.motor[0]->canMd_motor->outEnable,(uint32_t)driver.errorAdcValue[0],(uint32_t)driver.errorAdcValue[1],(uint32_t)driver.errorAdcValue[2]);
    		//serial.printf("%d,%d,%d,%d,%d\n\r",driver.driveStat,canMotor[0].outDuty16,canMotor[1].outDuty16,canMD[0].outDuty,canMD[1].outDuty);
    		if(sw[0].read() == 0){
    			canMotor[0].duty(-0.5);
    			canMotor[1].duty(-0.5);
    			serial.printf("ccw");
    		}else if(sw[1].read() == 0){
    			canMotor[0].duty(0.5);
    			canMotor[1].duty(0.5);
    			serial.printf("cw");
    		}else{
    			canMotor[0].free();
    			canMotor[1].free();
    			serial.printf("fr");
    		}
    	}

    }
}
