#include "KawaRoboMain.h"

void KawaRobo::setup(USART &serialSet,SBUS &sbusSet,SerialArduino &saSet,NCP5359 &motor0,NCP5359 &motor1,NCP5359 &motor2,NCP5359 &motor3){
	this->serial = &serialSet;
	this->sbus = &sbusSet;
	this->sa   = &saSet;

	this->motor[0] = &motor0;
	this->motor[1] = &motor1;
	this->motor[2] = &motor2;
	this->motor[3] = &motor3;

	printValueSelect = 0;

	for(int i=0;i<4;i++){
		target[i].setup(0,1,0);
	}
	motorInvertTime = 0;
	motorInvertFlag = 0;

	robotX.setup(0,1,0);
	robotY.setup(0,1,0);
	robotR.setup(0,0.3,0);

	robotY.errorLimitInt(1.45,36.5);
	robotX.errorLimitInt(1.45,36.5);
}

void KawaRobo::uiSetup(Switch &sw0,Switch sw1,LED &led0,LED &led1,LED &led2,LED &led3){
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

/*

	motor[0]->cycle();
	motor[1]->cycle();
	motor[2]->cycle();
	motor[3]->cycle();
*/
	led[0]->cycle();
	led[1]->cycle();
	led[2]->cycle();
	led[3]->cycle();

	//*/

	sw[0]->cycle();
	sw[1]->cycle();
/*
	for(int i=0;i<4;i++){
		motor[i]->cycle();
		led[i]->cycle();
	}//*/



	/**********************���T�C�N���񂷃}����KRM****************************/

	if(millis() > printTime){												//print����
		printTime += KRM_PRINT_CYCLE;

		//serial->printf("v = %dmv,",(int)(sa->read(3) * 14.925));

		//serial->printf("%d",(int)(motor[0]->outDuty*100));
		//serial->printf("sa d0 = %d,",sa->read(0));
		//serial->printf("%d,",(int)(target[0].output32()));
		//serial->printf("run = %4d,rev = %4d,arm = %4d,",(int)(runRead()*100),(int)(revRead()*100),(int)(armRead()*100));
		//serial->printf("%4d,%4d,%4d",sbus->read(KRM_CHANNEL_RUN) - 1024,sbus->read(KRM_CHANNEL_REV) - 1024,sbus->read(KRM_CHANNEL_MODE) - 1024);
		//serial->printf(" 4-7 = %4d,%4d,%4d,%4d,",sbus->read(4),sbus->read(5),sbus->read(6),sbus->read(7));
		//serial->printf("%d,%d,%d",robotX.output32(),robotY.output32(),robotR.output32());
		/*serial->printf("%4d",(int)(runRead()*100));
		for(int i = 0;i<4;i++){
			serial->printf("%4d,",(int)(motor[i]->outDuty*100));
		}//*/
		for(int i=0;i<8;i++){
			serial->printf("%5d,",sa->read(i));
		}//*/
		serial->printf("\n\r");
	}

	if(sa->update){
		sa->update = 0;
		gyroSpeed = (sa->read(0) - 0x7FFF) >> 4;
	}else if(millis() - sa->lastReceiveTime > KRM_SA_TIMEOUT_TOLERANCE){

	}

	if(sbus->update){														//���{�b�g�̐��������sbus�̎�M�����ˑ�
		sbus->update = 0;

		if(sbus->read(KRM_CHANNEL_MODE) > 1245){							//�v���|�̃g�O���X�C�b�`�̏�Ԃɂ���ē��샂�[�h��ύX����
			if(mode != KRM_MODE_RUN){
				motorEnable();
				serial->printf("\n\rmode = RUN\n\r\n\r");
				led[0]->interval(1000);
				led[1]->interval(1000);
			}
			mode = KRM_MODE_RUN;

		}else if(sbus->read(KRM_CHANNEL_MODE) > 805){
			if(mode != KRM_MODE_TEST){
				motorDisable();
				serial->printf("\n\rmode = TEST\n\r\n\r");
				led[0]->interval(50);
				led[1]->interval(1000);
			}
			mode = KRM_MODE_TEST;

		}else{
			if(mode != KRM_MODE_STOP){
				motorDisable();
				serial->printf("\n\rmode = STOP\n\r\n\r");
				led[0]->interval(50);
				led[1]->interval(1000);
			}
			mode = KRM_MODE_STOP;

		}

		controlCycle();

	}else if(millis() - sbus->lastReceiveTime > KRM_SBUS_TIMEOUT_TOLERANCE){		//��M�ł��ĂȂ���Ζ�����
		if(mode != KRM_MODE_TIMEOUT){
			motorDisable();
			serial->printf("\n\rmode = TIME OUT\n\r\n\r");
			led[0]->interval(50);
			led[1]->interval(50);
		}
		mode = KRM_MODE_TIMEOUT;
	}

	if(millis() > revReadTime){
		revReadTime += KRM_ENC_REV_READ_INT;
	}
}


void KawaRobo::controlCycle(){		//
	switch(mode){
	case KRM_MODE_RUN:
		run();
		break;

	case KRM_MODE_TEST:
		test();
		break;

	case KRM_MODE_STOP:

		break;

	}
}

void KawaRobo::test(){
	/*
	robotR.input((float)stickRead(0)/KRM_STICK_MAX);
	robotX.input(runRead());
	robotY.input(revRead());//*/

	robotR.input(revRead());
	robotX.input(runRead()*sin(robotR.outputF()));
	robotY.input(runRead()*cos(robotR.outputF()));

/*
	sa->write(0,robotR.output32());
	sa->write(1,robotY.output32());
	sa->write(2,sbus->read(2));
	sa->write(3,robotX.output32());
//*/
}

void KawaRobo::run(){
	target[0].measuredValue((float)(sbus->read(KRM_CHANNEL_RUN) - 1024)/100);

/*
	for(int i=0;i<4;i++){
		motor[i]->duty(0.5);
	}//*/

	/*
	motor[0]->duty(runRead());
	motor[1]->duty(runRead());
	motor[2]->duty(runRead());
	motor[3]->duty(runRead());
	*/

	if(millis() > motorInvertTime){
		float outDuty;
		motorInvertTime = millis() + (sbus->read(7) - 140);

		if(motorInvertFlag){
			motorInvertFlag = 0;
			outDuty = 1.0;

		}else{
			motorInvertFlag = 1;
			outDuty = -1.0;
		}



		if(sbus->read(5) < 500){
			outDuty *= 0.1;
		}else if(sbus->read(5) < 1500){
			outDuty *= 0.5;
		}

		for(int i=0;i<4;i++){
			motor[i]->duty(outDuty);
		}
	}

	//*/
	//motor[KRM_Motor_Arm]->duty(armRead());	//�A�[������
	led[2]->write(armRead());

	if(sbus->read(5) > 1000){
		led[3]->write(1);
	}else{
		led[3]->write(0);
	}

	/*
	for(int i=0;i<4;i++){
		motor[i]->duty(0.5);
	}//*/
}

void KawaRobo::motorDisable(){
	sa->write(0,0);		//motor disable
	for(int i=0;i<4;i++){
		motor[i]->duty(0);
	}
}

void KawaRobo::motorEnable(){
	sa->write(0,1);		//motor enable
	for(int i=0;i<4;i++){
		motor[i]->duty(0);
	}
}

float KawaRobo::runRead(){
	return (float)stickRead(KRM_CHANNEL_RUN)/KRM_STICK_MAX;
}

float KawaRobo::armRead(){
	return (float)(sbus->read(KRM_CHANNEL_ARM) - 144)/1760;
}

float KawaRobo::revRead(){
	return (float)stickRead(KRM_CHANNEL_REV)/KRM_STICK_MAX;
}

int KawaRobo::stickRead(uint16_t channel,uint16_t offset){
	int data = sbus->read(channel) - offset;

	if(data >= 0){
		if(data < KRM_STICK_TOLERANCE){
			data = 0;
		}else{
			data -= KRM_STICK_TOLERANCE;
		}
	}else{
		if(data > -KRM_STICK_TOLERANCE){
			data = 0;
		}else{
			data += KRM_STICK_TOLERANCE;
		}
	}

	if(data > KRM_STICK_MAX)data = KRM_STICK_MAX;
	if(data < -KRM_STICK_MAX)data = -KRM_STICK_MAX;

	return data;
}
