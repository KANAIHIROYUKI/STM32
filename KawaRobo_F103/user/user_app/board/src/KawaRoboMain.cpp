#include "KawaRoboMain.h"

void KawaRobo::setup(USART &serialSet,SBUS &sbusSet,SerialArduino &saSet,DutyOut &motor0,DutyOut &motor1,DutyOut &motor2,DutyOut &motor3){
	this->serial = &serialSet;
	this->sbus = &sbusSet;
	this->sa   = &saSet;

	this->motor[0] = &motor0;
	this->motor[1] = &motor1;
	this->motor[2] = &motor2;
	this->motor[3] = &motor3;

	printValueSelect = 0;
}

void KawaRobo::uiSetup(Switch &sw0,Switch sw1,GPIO &led0,GPIO &led1,GPIO &led2,GPIO &led3){
	this->sw[0] = &sw0;
	this->sw[1] = &sw1;

	this->led[0] = &led0;
	this->led[1] = &led1;
	this->led[2] = &led2;
	this->led[3] = &led3;
}

void KawaRobo::cycle(){

	sbus->cycle();
	sa->cycle();

	motor[0]->cycle();
	motor[1]->cycle();
	motor[2]->cycle();
	motor[3]->cycle();

	/**********************↑サイクル回すマン↓KRM****************************/

	if(sa->update){
		gyroSpeed = (sa->read(0) - 0x7FFF) >> 4;	//
	}

	if(sbus->update){
		sbus->update = 0;
		controlCycle();
	}

	if(sbus->read(KRM_CHANNEL_MODE) > 1245){			//プロポのトグルスイッチの状態によって動作モードを変更する
		mode = KRM_MODE_RUN;
	}else if(sbus->read(KRM_CHANNEL_MODE) > 805){
		mode = KRM_MODE_TEST;
	}else{
		mode = KRM_MODE_STOP;
	}

	if(millis() - sbus->lastReceiveTime > KRM_TIMEOUT_TOLERANCE){
		mode = KRM_MODE_TIMEOUT;
	}

	if(millis() > printTime){							//print周期
		printTime += KRM_PRINT_CYCLE;

		led[0]->toggle();

		serial->printf("mode:");
		switch(mode){
		case KRM_MODE_RUN:
			serial->printf("run ");
			break;

		case KRM_MODE_TEST:
			serial->printf("test");
			break;

		case KRM_MODE_STOP:
			serial->printf("stop");
			break;
		case KRM_MODE_TIMEOUT:
			serial->printf("Tout");
			break;
		}

		serial->printf("sa d0 = %d,",sa->read(0));

		serial->printf("%4d,%4d,%4d",sbus->read(KRM_CHANNEL_RUN) - 1024,sbus->read(KRM_CHANNEL_REV) - 1024,sbus->read(KRM_CHANNEL_ARM) - 1024);

		serial->printf("\n\r");
	}
}


void KawaRobo::controlCycle(){

	if(mode == KRM_MODE_STOP){
		for(int i=0;i<4;i++){
			motor[i]->duty(0);
		}
	}

	if(mode == KRM_MODE_RUN)run();
}

void KawaRobo::run(){

}
