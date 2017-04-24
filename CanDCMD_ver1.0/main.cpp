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

    		serial.printf("%d,%d,%d\n\r",isoIn.read(0),canVoltage.read(),(uint32_t)driver.vbattRead());

    		/*serial.printf("%d,%d,%d\n\r",isoIn.read(0),isoIn.read(1),isoIn.read(2));*/
    		//serial.printf("%d,%d,%d,%d,%d,%d,%d\n\r",(uint32_t)(millis() - isoIn.requestTime),(uint32_t)isoIn.receiveTime[0],isoIn.read(0),(uint32_t)isoIn.receiveTime[1],isoIn.read(1),(uint32_t)isoIn.receiveTime[2],isoIn.read(2));
    		//serial.printf("%d,%d,%d,%d\n\r",(uint32_t)canSwitch[0].lastReceiveTime,(uint32_t)canSwitch[1].lastReceiveTime,(uint32_t)canSwitch[2].lastReceiveTime,(uint32_t)canSwitch[3].lastReceiveTime);

    		//serial.printf("%d,%d,%d,%d\n\r",(uint32_t)canSw[0].canData[0],(uint32_t)canSw[1].canData[0],(uint32_t)canSw[2].canData[0],(uint32_t)canSw[3].canData[0]);
    		//serial.printf("%d,%d,%d,%d\n\r",(uint32_t)canSw[0].sw->read(),(uint32_t)canSw[1].sw->read(),(uint32_t)canSw[2].sw->read(),(uint32_t)canSw[3].sw->read());
    		//serial.printf("%d,%d,%d,%d\n\r",(uint32_t)canSw[0].intervalTime,(uint32_t)canSw[1].intervalTime,(uint32_t)canSw[2].intervalTime,(uint32_t)canSw[3].intervalTime);
    		//serial.printf("%x,%x,%x\n\r",canSw.canData[0],canSwitch.readStat,canSwitch.read());

    		/*
    		switch(debugMode){
    		case 0:
    			serial.printf("%6d,%4d,%6d,%4d\n\r",(uint32_t)canMD[0].lastReceiveTime,canMD[0].outDuty,(uint32_t)canMD[1].lastReceiveTime,canMD[1].outDuty);

    			break;
    		case 1:
    			if(swPin[0].read() == 0){
    				canMotor[1].duty(0.2);
    				canMotor[0].duty(0.2);
    			}else if(swPin[1].read() == 0){
    				canMotor[1].duty(-0.2);
    				canMotor[0].duty(-0.2);
    			}else{
    				canMotor[0].free();
    				canMotor[1].free();
    			}

    			serial.printf("%5d,%5d,%6d,%6d\n\r",motor[0].outDuty,motor[1].outDuty,enc[0].read(),enc[1].read());
    			break;
    		case 2:
    			if(sw[0].readStat && sw[0].read() == 0)debugMode++;
    			serial.printf("enc A = %6d, B = %6d\n\r",(int32_t)canEncoder[0].read(),(int32_t)canEncoder[1].read());
    			break;
    		case 3:
    			if(sw[0].readStat && sw[0].read() == 0)debugMode=2;
    			serial.printf("sw stat = 0x%x,read = 0x%x\n\r",canSwitch.readStat,canSwitch.read());
    			break;
    		case 4:

    			break;
    		}*/


    	}

    }
}
