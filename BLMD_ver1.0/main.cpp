
#include "main.hpp"

int main(void)
{
	setup();
	//cs.itSetup();
	//cs.start();

	//sys.wdgSetup(1000);	//5ms
	timer.reset();
	TIM_ITConfig(TIM2,TIM_IT_Update,DISABLE);

	outPower = 0;

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
    			serial.printf("%d,%d\n\r",(uint32_t)driveStat,(uint32_t)interruptFuncCnt);
    		}else{
    			outDegree = hallToDegree[(hall[2].read() << 2) + (hall[1].read() << 1) + (hall[0].read())];
    			serial.printf("%d,%d\n\r",outDegree,(uint32_t)(outPower*1000));
    		}

    		outPower = (float)((float)ADDRESS/(float)15.0);

    		driveStat = 0;
    		interruptFuncCnt = 0;
    	}
	}
}



inline void hallInterruptFunc(){
	if(driveMode == 1){
		interruptTime = timer.read();
		timer.reset();
		outDegree = hallToDegree[(hall[2].read() << 2) + (hall[1].read() << 1) + (hall[0].read())];

		if(driveEnable == 1){
			bldc.degree(outDegree,outPower);
		}else{
			bldc.free();
		}

		if(outDegree < 60){
			t_time[0] = interruptTime;
		}else if(outDegree < 120){
			t_time[1] = interruptTime;
		}else if(outDegree < 180){
			t_time[2] = interruptTime;
		}else if(outDegree < 240){
			t_time[3] = interruptTime;
		}else if(outDegree < 300){
			t_time[4] = interruptTime;
		}else{
			t_time[5] = interruptTime;
		}

		return;
	}

	interruptTime = timer.read();
	timer.reset();
	outDegree = hallToDegree[(hall[2].read() << 2) + (hall[1].read() << 1) + (hall[0].read())];

	if(trigger.stat() == 1){			//ホールセンサの方が速かったとき
		driveStat++;
		trigger.reset();
		trigger.stop();
		if(driveEnable == 1){
			bldc.degree(outDegree,outPower);
		}else{
			bldc.free();
		}
	}

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


	if(interruptTime < 300){	//高回転時に進角5%=18°
		//interruptTime*=ADVANCED_ANGLE;
	}

	//interruptTime *= 0.95;

	interruptTime -= 10;	//制御時間オフセット

	trigger.reset();
	trigger.ccSetup(1,interruptTime);
}


void TIM2_TIM_CC1_IRQ(){
	trigger.stop();
	if(driveEnable == 1){
		bldc.degree(outDegree + 60,outPower);
	}else{
		bldc.free();
	}
	interruptFuncCnt++;
	interruptCnt = timer.read() - setTime;
	trigger.reset();
}
