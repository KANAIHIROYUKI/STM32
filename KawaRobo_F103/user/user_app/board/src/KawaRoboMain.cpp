#include "KawaRoboMain.h"

void KawaRobo::setup(USART &serialSet,SBUS &sbusSet,SerialArduino &saSet,NCP5359 &motor0,NCP5359 &motor1,NCP5359 &motor2,NCP5359 &motor3){
	this->serial = &serialSet;
	this->sbus = &sbusSet;
	this->sa   = &saSet;

	this->motor[0] = &motor0;
	this->motor[1] = &motor1;
	this->motor[2] = &motor2;
	this->motor[3] = &motor3;

	motor[1]->dutyLimit(0.2);

	printValueSelect = 0;

	for(int i=0;i<4;i++){
		target[i].setup(0,1,0);
	}
	motorInvertTime = 0;
	motorInvertFlag = 0;

	dispValue = 0;

	robotX.setup(0,1,0);
	robotY.setup(0,1,0);
	robotR.setup(0,0.3,0);

	robotY.errorLimitInt(1.45,36.5);
	robotX.errorLimitInt(1.45,36.5);

	armPot.setup(0,0,1);

}

void KawaRobo::uiSetup(Switch &sw0,Switch &sw1,LED &led0,LED &led1,LED &led2,LED &led3){
	this->sw[0] = &sw0;
	this->sw[1] = &sw1;

	this->led[0] = &led0;
	this->led[1] = &led1;
	this->led[2] = &led2;
	this->led[3] = &led3;
}

void KawaRobo::sensorSetup(ADC &adc0,ADC &adc1,ADC &adc2,ADC &adc3){
	this->analog[0] = &adc0;
	this->analog[1] = &adc1;
	this->analog[2] = &adc2;
	this->analog[3] = &adc3;
}

void KawaRobo::cycle(){

	sbus->cycle();
	sa->cycle();

	motor[0]->cycle();
	motor[1]->cycle();
	motor[2]->cycle();
	motor[3]->cycle();

	led[0]->cycle();
	led[1]->cycle();
	led[2]->cycle();
	led[3]->cycle();

	sw[0]->cycle();
	sw[1]->cycle();



	/**********************↑サイクル回すマン↓KRM****************************/

	display();

	if(sa->update){
		sa->update = 0;
		gyroSpeed = (sa->read(0) - 0x7FFF) >> 4;
	}else if(millis() - sa->lastReceiveTime > KRM_SA_TIMEOUT_TOLERANCE){

	}

	if(sbus->update){														//ロボットの制御周期はsbusの受信周期依存
		sbus->update = 0;

		if(millis() - sa->lastReceiveTime < 100){
			if((sa->read(3) * 14.925) > 10000){		//Arduinoからの受信がある､かつ電圧OK

				if(sbus->read(KRM_CHANNEL_MODE) > 1245){							//プロポのトグルスイッチの状態によって動作モードを変更する
					if(mode != KRM_MODE_RUN){
						motorEnable();
						serial->printf("\n\rmode = RUN\n\r\n\r");
						led[0]->interval(1000);
						led[1]->interval(1000);
						led[2]->interval(1000);
					}
					mode = KRM_MODE_RUN;

				}else if(sbus->read(KRM_CHANNEL_MODE) > 805){
					if(mode != KRM_MODE_TEST){
						motorDisable();
						serial->printf("\n\rmode = TEST\n\r\n\r");
						led[0]->interval(50);
						led[1]->interval(1000);
						led[2]->interval(1000);
					}
					mode = KRM_MODE_TEST;

				}else{
					if(mode != KRM_MODE_STOP){
						motorDisable();
						serial->printf("\n\rmode = STOP\n\r\n\r");
						led[0]->interval(50);
						led[1]->interval(1000);
						led[2]->interval(1000);
					}
					mode = KRM_MODE_STOP;

				}
			}else{
				if(mode != KRM_MODE_STOP){
					motorDisable();
					serial->printf("\n\rbatt under voltage\n\rmode = STOP\n\r\n\r");
					led[0]->interval(50);
					led[1]->interval(1000);
					led[2]->interval(50);
				}
				mode = KRM_MODE_STOP;
			}
		}else{
			if(mode != KRM_MODE_STOP){
				motorDisable();
				serial->printf("\n\rsa com lost\n\rmode = STOP\n\r\n\r");
				led[0]->interval(50);
				led[1]->interval(1000);
				led[2]->interval(50);
			}
			mode = KRM_MODE_STOP;
		}

		controlCycle();

	}else if(millis() - sbus->lastReceiveTime > KRM_SBUS_TIMEOUT_TOLERANCE){		//受信できてなければ無条件
		if(mode != KRM_MODE_TIMEOUT){
			motorDisable();
			serial->printf("\n\rmode = TIME OUT\n\r\n\r");
			led[0]->interval(50);
			led[1]->interval(50);
			led[2]->interval(50);
		}
		mode = KRM_MODE_TIMEOUT;
	}

	if(millis() > revReadTime){
		revReadTime += KRM_ENC_REV_READ_INT;
	}
}

void KawaRobo::display(){
	if(millis() > printTime){												//print周期
		printTime += KRM_PRINT_CYCLE;

		if(sw[0]->readStat == 1 && sw[0]->read() == 0){
			dispValue++;
		}

		if(sw[1]->readStat == 1 && sw[1]->read() == 0){
			dispValue--;
		}

		switch(dispValue){
		case 0:
			serial->printf("sa ");
			for(int i=0;i<12;i++){
				serial->printf("%5d,",sa->read(i));
			}
			break;

		case 1:
			serial->printf("sbus ");
			for(int i=0;i<8;i++){
				serial->printf("%5d,",sbus->read(i));
			}
			break;

		case 2:
			serial->printf("v = %dmv,",(int)(sa->read(3) * 14.925));
			break;

		case 3:
			serial->printf("robot %d,%d,%d",robotX.output32(),robotY.output32(),robotR.output32());
			break;

		case 4:
			serial->printf("trg %5d,%5d,%5d",target[0].output32(),target[1].output32(),target[2].output32());
			break;

		case 5:
			armPot.measuredValue((float)analog[0]->read()/4095);

			serial->printf("analog ");
			serial->printf("%4d,",armPot.output32());
			break;

		default:
			dispValue = 0;
		}

		serial->printf("\n\r");
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

/*
	sa->write(0,robotR.output32());
	sa->write(1,robotY.output32());
	sa->write(2,sbus->read(2));
	sa->write(3,robotX.output32());
//*/
}

void KawaRobo::run(){

	target[0].measuredValue(runRead());
	target[1].measuredValue(revRead());

	robotR.input(((float)sa->read(0)/32767));
	robotX.input(runRead()*sin(robotR.outputF()));
	robotY.input(runRead()*cos(robotR.outputF()));

	/*
	motor[0]->duty((runRead() + ((float)sa->read(0)/32767)) / 2);
	motor[3]->duty((runRead() - ((float)sa->read(0)/32767)) / 2);//*/

	motor[0]->duty((runRead() + revRead()) / 2);
	motor[3]->duty((runRead() - revRead()) / 2);
	motor[1]->duty(armRead());
	sa->write(0,1);
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
	return (float)stickRead(KRM_CHANNEL_ARM,1465)/(KRM_STICK_MAX/3);
	//return (float)(sbus->read(KRM_CHANNEL_ARM) - 144)/1760;
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
