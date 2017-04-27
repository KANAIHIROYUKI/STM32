#include "main.hpp"

int main(void)
{
	setup();

	serial.printf("pinnum = 0x%x\n\r",canSw.switchNumber);

	//sys.wdgSetup(500);	//50ms


    while(1){
    	sys.cycle();

    	sw[0].cycle();
    	sw[1].cycle();

    	canEnc[1].cycle();
    	canEnc[0].cycle();

    	canSw.cycle();

    	canVol.cycle();

    	driver.cycle();


    	if(intervalTimer <= millis()){

    		intervalTimer = millis() + IntervalTime;

    		//serial.printf("%d,%d,%d\n\r",isoIn.read(0),isoIn.read(1),isoIn.read(2));
    		//serial.printf("%d,%d,%d,%d,%d\n\r",(uint32_t)driver.vbattRead(),(uint32_t)driver.currentRread(0),(uint32_t)driver.currentRread(1),driver.currentValue[0],driver.currentValue[1]);
    		serial.printf("vval=%d,error=%d,en=%d,v0=%d,v1=%d,v2=%d\n\r",canVoltage.read(),driver.driveError,(uint32_t)driver.motor[0]->canMd_motor->outEnable,(uint32_t)driver.errorAdcValue[0],(uint32_t)driver.errorAdcValue[1],(uint32_t)driver.errorAdcValue[2]);
    	}

    }
}
