#include "main.hpp"

int main(void)
{
	setup();

	//serial.printf("pinnum = 0x%x\n\r",canSw.switchNumber);
	serial.printf("canAddress\n\r sw = 0x%x,enc0 = 0x%x,enc1 = 0x%x,md0 = 0x%x,md1 = 0x%x\n\r",canSw.canAddress,canEnc[0].canEnc_address,canEnc[1].canEnc_address,canMD[0].canMd_address[0],canMD[1].canMd_address[0]);

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

    	canSw.cycle();
    	//canSw[2].cycle();
    	//canSw[3].cycle();

    	printStat = 0;

    	if(intervalTimer <= millis()){
    		intervalTimer = millis() + IntervalTime;
    		//serial.printf("%d,%d,%d,%d\n\r",(uint32_t)canSwitch[0].lastReceiveTime,(uint32_t)canSwitch[1].lastReceiveTime,(uint32_t)canSwitch[2].lastReceiveTime,(uint32_t)canSwitch[3].lastReceiveTime);

    		//serial.printf("%d,%d,%d,%d\n\r",(uint32_t)canSw[0].canData[0],(uint32_t)canSw[1].canData[0],(uint32_t)canSw[2].canData[0],(uint32_t)canSw[3].canData[0]);
    		//serial.printf("%d,%d,%d,%d\n\r",(uint32_t)canSw[0].sw->read(),(uint32_t)canSw[1].sw->read(),(uint32_t)canSw[2].sw->read(),(uint32_t)canSw[3].sw->read());
    		//serial.printf("%d,%d,%d,%d\n\r",(uint32_t)canSw[0].intervalTime,(uint32_t)canSw[1].intervalTime,(uint32_t)canSw[2].intervalTime,(uint32_t)canSw[3].intervalTime);
    		//serial.printf("%x,%x,%x\n\r",canSw.canData[0],canSwitch.readStat,canSwitch.read());

    		switch(debugMode){
    		case 0:

    			//serial.printf("%d,%d,%d,0x%x,0x%x\n\r",(uint32_t)canEnc[0].canEnc_interval,(uint32_t)canSw.intervalTime,canSw.interruptCnt,canSw.canData[0],canSw.canData[1]);

    			if(sw[0].read() == 0){
    				serial.printf("enc0,tt=%6d,cnt=%6d,en1,tt=%6d,cnt=%6d\n\r",(uint32_t)canEnc[0].canEnc_enc->read(),(uint32_t)canEnc[0].canEnc_intervalTimer,(uint32_t)canEnc[1].canEnc_enc->read(),(uint32_t)canEnc[1].canEnc_intervalTimer);
    			}else if(sw[1].read() == 0){
    				serial.printf("md,lrt=%6d,dt=%4d,rt=%6d,rt=%4d\n\r",(uint32_t)canMD[0].lastReceiveTime,canMD[0].outDuty,(uint32_t)canMD[1].lastReceiveTime,canMD[1].outDuty);
    			}else{
    				serial.printf("sw,it=%d,0=0x%x,d1=0x%x,gpio=0x%x,%d,%d,%d,%d\n\r",(uint32_t)canSw.intervalTime,canSw.sendStat,canSw.pinStat,canSw.gpioRead,canSw.sw[0]->gpio->read(),canSw.sw[1]->gpio->read(),canSw.sw[2]->gpio->read(),canSw.sw[3]->gpio->read());
    			}

    			break;
    		case 1:
    			if(swPin[0].read() == 0){
    				canMotor[1].duty(0.1);
    				canMotor[0].duty(0.1);
    			}else if(swPin[1].read() == 0){
    				canMotor[1].duty(-0.1);
    				canMotor[0].duty(-0.1);
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
    			if(sw[0].readStat && sw[0].read() == 0)debugMode--;
    			serial.printf("sw stat = 0x%x,read = 0x%x\n\r",canSwitch.readStat,canSwitch.read());
    			break;
    		case 4:

    			break;
    		}


    	}
    	if(canRxFlag && debugMode == 0){
    		canRxFlag = 0;
    		//serial.printf("0x%x,%d,%d,\n\r",can1.rxMessage.StdId,can1.rxMessage.Data[0],can1.rxMessage.Data[1]);
    	}

    }
}
