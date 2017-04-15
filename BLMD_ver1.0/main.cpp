
#include "main.hpp"

int main(void)
{
	setup();
	//cs.itSetup();
	//cs.start();

	//sys.wdgSetup(1000);	//5ms
	timer.reset();
	TIM_ITConfig(TIM2,TIM_IT_Update,DISABLE);


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

    		t_ave /= t_cnt;

    		serial.printf("a%d,n%d,x%d\n\r",(uint32_t)t_ave,(uint32_t)t_min,(uint32_t)t_max);
    		driveStat = 0;
    		interruptFuncCnt = 0;

    		t_cnt = 0;
    		t_ave = 0;
    		t_max = 0;
    		t_min = 10000000;
    	}
	}
}



inline void hallInterruptFunc(){
	if(driveMode == 1){
		interruptTime = timer.read();
		timer.reset();
		outDegree = hallToDegree[(hall[2].read() << 2) + (hall[1].read() << 1) + (hall[0].read())];

		if(driveEnable == 1){
			bldc.degree(outDegree,0.2);
		}else{
			bldc.free();
		}
		t_cnt++;
		if(t_min > interruptTime)t_min = interruptTime;
		if(t_max < interruptTime)t_max = interruptTime;
		t_ave+= interruptTime;

		return;
	}

	interruptTime = timer.read();
	timer.reset();
	outDegree = hallToDegree[(hall[2].read() << 2) + (hall[1].read() << 1) + (hall[0].read())];

	t_cnt++;
	if(t_min > interruptTime)t_min = interruptTime;
	if(t_max < interruptTime)t_max = interruptTime;
	t_ave+= interruptTime;
	/*if(trigger.stat() == 1){			//ホールセンサの方が速かったとき
		driveStat++;
		trigger.reset();
		TIM_ITConfig(TIM2,TIM_IT_CC1,DISABLE);
		TIM_Cmd(TIM2,DISABLE);
		//trigger.stop();
		if(driveEnable == 1){
			bldc.degree(outDegree,0.5);
		}else{
			bldc.free();
		}
		return;
	}*/

	if(interruptTime < 300){	//高回転時に進角5%=18°
		//interruptTime*=ADVANCED_ANGLE;
	}

	interruptTime -= CONTROL_ADJUST_TIME;	//制御時間オフセット

	if(interruptTime > 5000){	//低回転時に角度加算して始動
		if(driveEnable == 1){
			bldc.degree(outDegree + 60,0.2);
		}else{
			bldc.free();
		}
	}else{
		trigger.reset();
		trigger.ccSetup(1,interruptTime);
	}
}


void TIM2_TIM_CC1_IRQ(){
	trigger.stop();
	if(driveEnable == 1){
		bldc.degree(outDegree,0.2);
	}else{
		bldc.free();
	}
	interruptFuncCnt++;
	interruptCnt = timer.read() - setTime;
	trigger.reset();
}
