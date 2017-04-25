#include "main.hpp"

int main(void)
{
	setup();

	serial.printf("pinnum = 0x%x\n\r",canSw.switchNumber);

	//sys.wdgSetup(500);	//50ms

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

    	//canMD[0].cycle();
    	//canMD[1].cycle();

    	canSw.cycle();
    	//canSw[2].cycle();
    	//canSw[3].cycle();

    	//isoIn.cycle();
    	canVol.cycle();

    	driver.cycle();

    	if(intervalTimer <= millis()){

    		intervalTimer = millis() + IntervalTime;
    		//serial.printf("%d,%d,%d,%d,%d\n\r",(uint32_t)driver.vbattRead(),(uint32_t)driver.currentRread(0),(uint32_t)driver.currentRread(1),driver.currentValue[0],driver.currentValue[1]);
    		serial.printf("%d,%d,%d\n\r",canVoltage.read(),driver.driveError,(uint32_t)driver.motor[0]->canMd_motor->outEnable);
    	}

    }
}
