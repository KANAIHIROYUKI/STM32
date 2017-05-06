#include "main.hpp"

int main(void)
{
	setup();
	sys.wdgSetup(500);	//50ms

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

    	driver.cycle();


    	if(intervalTimer <= millis()){

    		intervalTimer = millis() + IntervalTime;

    		if(debugMode == 1){
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

    		}else if(debugMode == 2){
        		if(sw[0].readStat == 1 && sw[0].read() == 0 && printValue > 0){
        			printValue--;
        		}
        		if(sw[1].readStat == 1 && sw[1].read() == 0 && printValue < 3){
        			printValue++;
        		}
    		}

    		switch(printValue){
    		case 0:
        		serial.printf("t=%d,canMD En %d%d,motorEn %d%d,",(uint32_t)(millis() - driver.adc->receiveTime[0]),driver.motor[0]->motorOutEnable,driver.motor[0]->motorOutEnable,driver.motor[0]->canMd_motor->outEnable,driver.motor[1]->canMd_motor->outEnable);
        		serial.printf("vval=%d,stat=%d,error=0x%x,%d\n\r",canVoltage.read(),driver.driveStat,driver.driveError,driver.driveErrorStat);

    			break;
    		case 1:
    			serial.printf("v=%d,c0=%d,c1=%d,cv0=%d,cv1=%d\n\r",(uint32_t)driver.vbattRead(),(uint32_t)driver.currentRread(0),(uint32_t)driver.currentRread(1),driver.currentValue[0],driver.currentValue[1]);

    			break;
    		case 2:
    			serial.printf("limit,%d,%d,%d,%d\n\r",limit[0].gpioRead(),limit[1].gpioRead(),limit[2].gpioRead(),limit[3].gpioRead());
    			break;
    		case 3:
    			//adcのデータぶんまわし
    			break;
    		}

/*

    		*/

    	}

    	if(printValue && driver.adcCycleOnetime()){
    		serial.printf("%d,%d,%d\n\r",driver.adc->value[0],driver.adc->value[1],driver.adc->value[2]);	//driverがdriveモードのときのみ
    	}

    }
}
