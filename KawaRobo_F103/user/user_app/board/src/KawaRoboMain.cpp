#include "KawaRoboMain.h"

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

	sensorUpdate();
	display();

	/**********************↑サイクル回すマン↓KRM****************************/

	if(sbus->update){														//ロボットの制御周期はsbusの受信周期依存
		sbus->update = 0;

		if(millis() - sa->lastReceiveTime < KRM_SA_TIMEOUT_TOLERANCE){
			if((sa->read(3) * adcToBattV) > 10000){		//Arduinoからの受信がある､かつ電圧OK

				if(sbus->read(KRM_CHANNEL_MODE) > 1245){							//プロポのトグルスイッチの状態によって動作モードを変更する
					if(mode != KRM_MODE_RUN){
						mode = KRM_MODE_RUN;

						motorEnable();
						serial->printf("\n\rmode = RUN\n\r\n\r");
						led[0]->interval(1000);
						led[1]->interval(1000);
						led[2]->interval(1000);
					}

				}else if(sbus->read(KRM_CHANNEL_MODE) > 805){
					if(mode != KRM_MODE_TEST){
						mode = KRM_MODE_TEST;

						motorDisable();
						serial->printf("\n\rmode = TEST\n\r\n\r");
						led[0]->interval(1000);
						led[1]->interval(1000);
						led[2]->interval(1000);
					}

				}else{
					if(mode != KRM_MODE_STOP){
						mode = KRM_MODE_STOP;

						motorDisable();
						serial->printf("\n\rmode = STOP\n\r\n\r");
						led[0]->interval(50);
						led[1]->interval(1000);
						led[2]->interval(1000);
					}

				}
			}else{
				if(mode != KRM_MODE_STOP){
					mode = KRM_MODE_STOP;

					motorDisable();
					serial->printf("\n\rbatt under voltage\n\rmode = STOP\n\r\n\r");
					led[0]->interval(50);
					led[1]->interval(1000);
					led[2]->interval(50);
				}
			}
		}else{
			if(mode != KRM_MODE_STOP){
				mode = KRM_MODE_STOP;

				motorDisable();
				serial->printf("\n\rsa com lost\n\rmode = STOP\n\r\n\r");
				led[0]->interval(50);
				led[1]->interval(1000);
				led[2]->interval(50);
			}
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

/*************************************************************************************/

void KawaRobo::display(){
	if(serial->available()){
		while(serial->available()){
			dispValue++;
			serial->read();
		}
	}

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
			dispValue = 6;

		case 1:
			serial->printf("sa ");
			for(int i=0;i<12;i++){
				serial->printf(",%5d",sa->read(i));
			}
			break;

		case 2:
			serial->printf("sbus ");
			for(int i=0;i<8;i++){
				serial->printf(",%5d",sbus->read(i));
			}
			break;

		case 3:
			serial->printf("v = ,%d,mv,",(int)(sa->read(3) * 14.925));
			break;

		case 4:
			serial->printf("robot armPID = ,%4d,robotR = ,%4d",armPID.output32(),robotR.output32());
			break;

		case 5:
			serial->printf("analog ");
			serial->printf("%4d,%4d,%d",(int)armDegree,armPID.output32(),potCnt);
			break;

		case 6:
			serial->printf("motot out ");
			serial->printf(",%d",(int)(motor[0]->outDuty*100));
			serial->printf(",%d",(int)(motor[1]->outDuty*100));
			serial->printf(",%d",(int)(motor[3]->outDuty*100));
			break;

		default:
			dispValue = 1;
		}

		serial->printf("\n\r");
	}
}

void KawaRobo::controlCycle(){		//
	armPotUpdate();

	switch(mode){
	case KRM_MODE_RUN:
		sa->write(0,1);
		run();
		break;

	case KRM_MODE_TEST:
		motorDisable();
		test();
		break;

	case KRM_MODE_STOP:
		motorDisable();

		break;

	}
}

/*************************************************************************************/

void KawaRobo::test(){

}

void KawaRobo::run(){

	motor[0]->duty((getRunPosition() + getRevPosition()) / 2);
	motor[3]->duty((getRunPosition() - getRevPosition()) / 2);
	motor[1]->duty(getArmPosition());
	armTarget.measuredValue(getArmPosition());

	armCurrent.measuredValue((motor[1]->outDuty*14) - 0.1);
}



/*************************************************************************************/

//制御周期ごとに呼ぶ｡S.BUSなかったら呼ばれないので注意
void KawaRobo::armPotUpdate(){
	float value = (float)((pot1Int/pot1Cnt) - (pot2Int/pot2Cnt))/8191;
	potCnt = pot1Cnt + pot2Cnt;
	pot1Cnt = 0;
	pot1Int = 0;
	pot2Cnt = 0;
	pot2Int = 0;
	armDegree = value * 270;

	armPID.measuredValue(armDegree - armTarget.outputF());
}

//制御周期関係なくぶんまわし
void KawaRobo::sensorUpdate(){
	if(analog[0]->converted()){
		pot1Int += analog[0]->peek();
		pot1Cnt++;
		analog[0]->start();
	}

	if(analog[1]->converted()){
		pot2Int += analog[1]->peek();
		pot2Cnt++;
		analog[1]->start();
	}

	if(sa->update){
		sa->update = 0;
		robotR.measuredValue((float)(sa->read(0) + gyroOffset)/39200);
	}
}

/***********************************初期設定関数群**************************************************/

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

	motorInvertTime = 0;
	motorInvertFlag = 0;

	dispValue = 1;

	robotR.setup(0,1,0);
	armPID.setup(1,0,0);
	armTarget.setup(0,1,0);

	armTarget.errorLimitInt(-135,135);

	armCurrent.setup(0,1,0);
	armCurrent.errorLimitInt(0,100);
}

void KawaRobo::uiSetup(Switch &sw0,Switch &sw1,LED &led0,LED &led1,LED &led2,LED &led3){
	this->sw[0] = &sw0;
	this->sw[1] = &sw1;

	this->led[0] = &led0;
	this->led[1] = &led1;
	this->led[2] = &led2;
	this->led[3] = &led3;

	led[0]->interval(50);
	led[1]->interval(50);
	led[2]->interval(50);
}

void KawaRobo::sensorSetup(ADC &adc0,ADC &adc1,ADC &adc2,ADC &adc3,int16_t _gyroOffset,float _adcToBatt){
	this->analog[0] = &adc0;
	this->analog[1] = &adc1;
	this->analog[2] = &adc2;
	this->analog[3] = &adc3;
	gyroOffset = _gyroOffset;
	adcToBattV = _adcToBatt;
}

/*************************************************************************************/

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

/*************************************************************************************/

float KawaRobo::getRunPosition(){
	return (float)getStickPosition(KRM_CHANNEL_RUN)/KRM_STICK_MAX;
}

float KawaRobo::getArmPosition(){
	return (float)getStickPosition(KRM_CHANNEL_ARM,1465)/(KRM_STICK_MAX/3);
}

float KawaRobo::getRevPosition(){
	return (float)getStickPosition(KRM_CHANNEL_REV)/KRM_STICK_MAX;
}


int KawaRobo::getStickPosition(uint16_t channel,uint16_t offset){
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
