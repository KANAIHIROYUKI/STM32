
#include "main.hpp"

int main(void)
{
	setup();
	//cs.itSetup();
	//cs.start();
	//setTime = timer.read();


	//sys.wdgSetup(1000);	//5ms
	while(1){

    	if(printTime <= millis()){
/*
    		if(sw[0].read() == 0){
    			if(interruptTime > 5000){
    				for(int i=0;i<100;i++){
    					outDegree = hallToDegree[(hall[2].read() << 2) + (hall[1].read() << 1) + (hall[0].read())];
        				bldc.degree(outDegree+30,0.9);
        				delay(1);
    				}

    			}
    			driveEnable = 1;

    		}else{
    			driveEnable = 0;
    		}
*/
    		while(sw[1].read() == 0){
				outDegree = hallToDegree[(hall[2].read() << 2) + (hall[1].read() << 1) + (hall[0].read())];
				//bldc.degree(outDegree ,0.5);
    		}

    		led[0].toggle();
    		printTime = millis() + PRINT_TIME;

    		serial.printf("%d\n\r",outDegree);
    	}
	}
}



inline void hallInterruptFunc(){
	TIM_ITConfig(TIM2,TIM_IT_Update,DISABLE);

	interruptTime = timer.read();
	timer.reset();

	outDegree = hallToDegree[(hall[2].read() << 2) + (hall[1].read() << 1) + (hall[0].read())];

	/*if(interruptTime > 5000){	//’á‰ñ“]Žž‚ÉŠp“x‰ÁŽZ‚µ‚ÄŽn“®
		outDegree+=10;
		phaseStat = 1;
	}else{
		phaseStat = 0;
	}*/

	if(interruptTime < 100){	//‚‰ñ“]Žž‚ÉiŠp5%=18‹
		//interruptTime*=ADVANCED_ANGLE;
	}

	//interruptTime -= CONTROL_ADJUST_TIME;	//§ŒäŽžŠÔƒIƒtƒZƒbƒg

	if(interruptTime > 65535){
		if(driveEnable == 1){
			bldc.degree(outDegree,0.5);
		}else{
			bldc.free();
		}
	}else{
		TIM_ITConfig(TIM2,TIM_IT_Update,DISABLE);
		TIM_Cmd(TIM2,DISABLE);

		TIM2->CNT = 0;

		TIM_TimeBaseInitTypeDef TimeBaseInitStructure;
		TIM_TimeBaseStructInit(&TimeBaseInitStructure);
		TimeBaseInitStructure.TIM_Period = 2000;
		TimeBaseInitStructure.TIM_Prescaler = 1000;
		TIM_TimeBaseInit(TIM2,&TimeBaseInitStructure);

		TIM_ARRPreloadConfig(TIM2,ENABLE);

		NVIC_InitTypeDef NVIC_InitStructure;
		NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);

		TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
		TIM_Cmd(TIM2,ENABLE);

		//trigger.triggerSetup(TIM2,65536);

	}
}


void TIM2_TRIGGER_Update_IRQ(){
	/*
	interruptCnt = timer.read() - interruptCnt;
	interruptFuncCnt++;
	if(driveEnable == 1){
		bldc.degree(outDegree,0.5);
	}else{
		bldc.free();
	}*/
	interruptCnt = timer.read();
	interruptFuncCnt++;
}
