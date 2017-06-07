
#include "main.hpp"

int main(void)
{
	setup();
	//cs.itSetup();
	//cs.start();

	//sys.wdgSetup(1000);	//5ms

	TIM_ITConfig(TIM2,TIM_IT_Update,DISABLE);

	outPower = 0;

	speedLimitTime = 1500;

	while(1){

    	if(printTime <= millis()){

    		if(sw[0].read() == 0){
    			driveMode = 0;
    			if(driveEnable == 0){
    				driveEnable = 1;
    				hallInterruptFunc();
    			}
    		}else{
        		if(sw[1].read() == 0){
        			driveMode = 1;
        			if(driveEnable == 0){
        				driveEnable = 1;
        				hallInterruptFunc();
        			}
        		}else{
        			driveEnable = 0;
        			trigger.stop();
        			outDegree = hallToDegree[(hall[2].read() << 2) + (hall[1].read() << 1) + (hall[0].read())];
        		}
    		}

    		led[0].toggle();

    		printTime = millis() + PRINT_TIME;
    		if(driveEnable == 1){
    			//serial.printf("%d,%d,%d,%d,%d,%d\n\r",(uint32_t)hallLowTime[0],(uint32_t)hallHighTime[0],(uint32_t)hallLowTime[1],(uint32_t)hallHighTime[1],(uint32_t)hallLowTime[2],(uint32_t)hallHighTime[2]);
    			serial.printf("%d,%d\n\r",(uint32_t)triggerCnt[0],(uint32_t)triggerCnt[1]);
    		}else{
    			outDegree = hallToDegree[(hall[2].read() << 2) + (hall[1].read() << 1) + (hall[0].read())];
    			serial.printf("%d,%d\n\r",outDegree,(uint32_t)(outPower*1000));
    		}

    		outPower = (float)((float)ADDRESS/(float)15.0);


    		triggerCnt[0] = 0;
    		triggerCnt[1] = 0;
    		driveStat = 0;
    	}
	}
}



inline void hallInterruptFunc(){
	//interruptTime = timer.read();
	//timer.reset();
	outDegree = hallToDegree[(hall[2].read() << 2) + (hall[1].read() << 1) + (hall[0].read())];

	if(hall[interruptHallNumber].read() == 1){
		hallRaiseTime[interruptHallNumber] = micros();
	}else{
		hallFallTime[interruptHallNumber] = micros();
	}

	if(hallRaiseTime[interruptHallNumber] > hallFallTime[interruptHallNumber]){
		hallLowTime[interruptHallNumber] = hallRaiseTime[interruptHallNumber] - hallFallTime[interruptHallNumber];
		interruptTime = hallLowTime[interruptHallNumber];
	}else{
		hallHighTime[interruptHallNumber] = hallFallTime[interruptHallNumber] - hallRaiseTime[interruptHallNumber];
		interruptTime = hallHighTime[interruptHallNumber];
	}

	if(driveMode == 0){
		if(interruptTime < 10000){

			if(triggerStat){
				triggerStat = 0;
				trigger.reset();
				outTriggerDegree[0] = outDegree;
				trigger.ccSetup(1,interruptTime/3);
			}else{
				triggerStat = 1;
				trigger.reset();
				outTriggerDegree[1] = outDegree;
				trigger.ccSetup(1,interruptTime/3);
			}
			return;
		}
	}


	if(driveEnable == 1){
		bldc.degree(outDegree,outPower);
	}else{
		bldc.free();
	}


	/*
	if(trigger.stat() == 1){			//ホールセンサの方が速かったとき
		driveStat++;
		trigger.reset();
		trigger.stop();
		if(driveEnable == 1){
			bldc.degree(outDegree,outPower);
		}else{
			bldc.free();
		}
	}*/

	/*if(outDegree < 60){
		interruptTime *= 1.04;
	}else if(outDegree < 120){
		interruptTime *= 0.94;
	}else if(outDegree < 180){
		interruptTime *= 1.0;
	}else if(outDegree < 240){
		interruptTime *= 1.31;
	}else if(outDegree < 300){
		interruptTime *= 1.08;
	}else{
		interruptTime *= 0.59;
	}*/
/*

	if(interruptTime < 300){	//高回転時に進角5%=18°
		//interruptTime*=ADVANCED_ANGLE;
	}
*/
	//interruptTime *= 0.95;
/*
	interruptTime -= 10;	//制御時間オフセット

	trigger.reset();
	trigger.ccSetup(1,interruptTime);*/
}


void TIM2_TIM_CC_IRQ(){

}

void TIM1_TIM_CC1_IRQ(){

}

void TIM2_TIM_CC1_IRQ(){
	trigger.stop();
	if(driveEnable == 1){
		bldc.degree(outTriggerDegree[1] + 60,outPower);
	}else{
		bldc.free();
	}
	trigger.reset();
	//triggerCnt++;
}
