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

    		led[2].toggle();
    		led[3].toggle();
    		led[4].toggle();
    		if(debugMode == 1){
        		if(sw[0].read() == 0){
        			canMotor[0].duty(-0.2);
        			canMotor[1].duty(-0.2);
        			serial.printf("ccw");
        		}else if(sw[1].read() == 0){
        			canMotor[0].duty(0.2);
        			canMotor[1].duty(0.2);
        			serial.printf("cw");
        		}else{
        			canMotor[0].duty(0);
        			canMotor[1].duty(0);
        			serial.printf("fr");
        		}

    		}else{
        		if(sw[0].readStat == 1 && sw[0].read() == 0 && printValue > 0){
        			printValue--;
        		}
        		if(sw[1].readStat == 1 && sw[1].read() == 0 && printValue < 4){
        			printValue++;
        		}
    		}

    		switch(printValue){
    		case 0:
        		serial.printf("t=%3d,v=%d,c=%3d,%3d",(uint32_t)(millis() - driver.adc->receiveTime[0]),isoIn.read(2),isoIn.read(1),isoIn.read(0));
        		serial.printf("stat=%d,error=0x%3x,%d\n\r",driver.driveStat,driver.driveError,driver.driveErrorStat);

    			break;
    		case 1:
    			serial.printf("cnt=%d,v=%d,c0=%d,c1=%d,cv0=%d,cv1=%d,vvMin=%d,cvMax=%d\n\r",driver.adc->resetCnt,(uint32_t)driver.vbattRead(),(uint32_t)driver.currentRread(0),(uint32_t)driver.currentRread(1),driver.currentValue[0],driver.currentValue[1],driver.vvMin,driver.cvMax);

    			break;
    		case 2:
    			serial.printf("limit,0x%d%d%d%d,enc=%6d,%6d\n\r",limit[3].gpioRead(),limit[2].gpioRead(),limit[1].gpioRead(),limit[0].gpioRead(),(uint32_t)canEnc[0].canEnc_enc->read(),(uint32_t)canEnc[1].canEnc_enc->read());
    			break;
    		case 3:
    			//adcのデータぶんまわし
    			break;
    		case 4:
    			serial.printf("%d,%d,%d\n\r",(uint32_t)canMD[0].lastReceiveTime,(uint32_t)canMD[1].lastReceiveTime,(uint32_t)can1.receiveCnt);
    			break;
    		case 5:
    			//canデータぶんまわし
    			break;
    		}

    		if(driver.adc->setupStat == 0 && driver.driveError){
    			serial.printf("\n\r\n\rdriver error = 0x%3x\n\r\n\r",driver.driveError);
    			driver.driveError = 0;
    		}
    	}

    	if(printValue == 3 && driver.adcCycleOnetime()){
    		serial.printf("%d,%d,%d,%d\n\r",driver.adc->setupStat,driver.adc->value[0],driver.adc->value[1],driver.adc->value[2]);	//driverがdriveモードのときのみ
    	}
    }
}
