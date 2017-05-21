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


    		if(driver.driveError&DE_UnderVoltage || driver.driveError&DE_OverVoltage || driver.driveError&DE_ADCLost || driver.driveError&DE_Unknown){
    			led[1].write(1);
    			led[3].write(1);
    		}else{
        		if(driver.driveError&DE_BreakFEToutAHigh || driver.driveError&DE_BreakFEToutALow || driver.driveError&DE_OCoutA){
        			led[1].write(1);
        		}else{
        			led[1].write(0);
        		}

        		if(driver.driveError&DE_BreakFEToutBHigh || driver.driveError&DE_BreakFEToutBLow || driver.driveError&DE_OCoutB){
        			led[3].write(1);
        		}else{
        			led[3].write(0);
        		}
    		}


    		if(debugMode == 1){
        		if(sw[0].read() == 0){
        			canMotor[0].duty(-0.2);
        			canMotor[1].duty(-0.2);
        			serial.printf("ccw");
        		}else if(sw[1].read() == 0){
        			canMotor[0].duty(0.2);
        			canMotor[1].duty(0.2);
        			serial.printf("cw ");
        		}else{
        			canMotor[0].duty(0);
        			canMotor[1].duty(0);
        			serial.printf("fr ");
        		}

    		}else{
        		if(sw[0].readStat == 1 && sw[0].read() == 0 && printValue > 0){
        			printValue--;
        		}
        		if(sw[1].readStat == 1 && sw[1].read() == 0 && printValue < 5){
        			printValue++;
        		}
    		}

    		switch(printValue){
    		case 0:
    			serial.printf("v min,%d,ave,%5d,c A=,%3d,B=,%3d\n\r",(int)(driver.printvMin*1000),(int)(driver.printvAve*1000),(int)(driver.printcMax[0]),(int)(driver.printcMax[1]));

    			break;
    		case 1:
    			serial.printf("v=,%5dmv,c=,%2d,%2d,cmx=,%5d,%5d,dt=,%+5d,%+5d\n\r",(uint32_t)(driver.printvAve*1000),(uint32_t)driver.printcAve[0],(uint32_t)driver.printcAve[1],(int)(driver.printcMax[0]),(int)(driver.printcMax[1]),canMD[0].outDuty/10,canMD[1].outDuty/10);

    			break;
    		case 2:
    			serial.printf("md rvt=,%6d,%6d,dty=,%+d,%+d,enc itx=,%d,%d,sw itv=,%d,snd=,%d\n\r",(uint32_t)canMD[0].lastReceiveTime,(uint32_t)canMD[1].lastReceiveTime,canMD[0].outDuty,canMD[1].outDuty,(uint32_t)canEnc[0].canEnc_interval,(uint32_t)canEnc[1].canEnc_interval,canSw.intervalTime,(uint32_t)canSw.lastSendTime);

    			break;
    		case 3:
    			serial.printf("limit,0x%d%d%d%d,%d%d%d%d,enc=%6d,%6d\n\r",limit[3].gpioRead(),limit[2].gpioRead(),limit[1].gpioRead(),limit[0].gpioRead(),limit[3].read(),limit[2].read(),limit[1].read(),limit[0].read(),(uint32_t)canEnc[0].canEnc_enc->read(),(uint32_t)canEnc[1].canEnc_enc->read());

    			break;
    		case 4:
    			//adcのデータぶんまわし

    			break;
    		case 5:
    			//canデータぶんまわし

    			break;
    		}

    		if(driver.adc->setupStat == 0 && driver.driveError){
    			serial.printf("\n\r\n\rdriver error = 0x%3x\n\r\n\r",driver.driveError);
    			driver.driveError = 0;
    		}

    		if(driver.driveStat >= DS_MotorBuzzerCurrentTest && driver.driveStat <= DS_MotorBuzzer){
    			serial.printf("%d,%d\n\r",(uint32_t)(driver.buzzerTest[ChannelCurrentA]*1000),(uint32_t)(driver.buzzerTest[ChannelCurrentB]*1000));
    		}
    	}

    	if(printValue == 5 && driver.adcCycleOnetime()){
    		serial.printf("%d,%d,%d,%d\n\r",driver.adc->setupStat,driver.adc->value[0],driver.adc->value[1],driver.adc->value[2]);	//driverがdriveモードのときのみ
    	}

    	if(printValue == 6 && can1.receiveCnt > 0){
    		serial.printf("rCnt=%d,id=0x%x,data=0x%x,0x%x",can1.receiveCnt,can1.rxMessage.StdId,can1.rxMessage.Data[0],can1.rxMessage.Data[1]);
    		can1.receiveCnt = 0;
    	}
    }
}
