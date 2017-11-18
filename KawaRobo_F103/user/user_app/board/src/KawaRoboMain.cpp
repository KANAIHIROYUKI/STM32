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

	armPotCycle();
	displayCycle();

	/**********************↑サイクル回すマン↓KRM****************************/

	if(sa->update){
		sa->update = 0;
		robotR.measuredValue(-(float)(sa->read(0) + gyroOffset)/39200);
	}

	if(sbus->update){														//SBUS受信時､ロボットの制御周期はsbusの受信周期依存
		sbus->update = 0;
		armPotUpdate();

		if(millis() - sa->lastReceiveTime < SA_TIMEOUT_TOLERANCE){
			if((sa->read(3) * adcToBattV) > battUnderVoltage){		//Arduinoからの受信がある､かつ電圧OK
				battUnderVoltage = BATT_UNDER_LIMIT;

				if(getTogglePosition(0) < 0){							//プロポのトグルスイッチの状態によって動作モードを変更する
					if(mode != MODE_RUN){
						mode = MODE_RUN;

						speakRequest = 19;


						motorEnable();
						serial->printf("\n\rmode = RUN\n\r\n\r");
						led[0]->interval(1000);
						led[1]->interval(1000);
						led[2]->interval(1000);

						robotPIDR.clear();		//いらん動きしないようにPIDコントローラーもリセット
						robotTargetR.clear();	//目標角度を0にする､現在の角度と目標角度の差を無くす
						robotR.clear();			//ジャイロ角度の積分値をリセット､現在角度を0度とする

						armPID.clear();			//いらん動きしないようにPIDコントローラーもリセット
						armTarget.clear();		//目標角度を0にする
						armTarget.errorInt = armDegree;		//現在の角度を目標角度にする
					}

				}else if(getTogglePosition(0) < 0.5){
					if(mode != MODE_TEST){
						mode = MODE_TEST;
						sa->write(1,0);
						speakRequest = 18;

						motorDisable();
						serial->printf("\n\rmode = TEST\n\r\n\r");
						led[0]->interval(1000);
						led[1]->interval(1000);
						led[2]->interval(1000);
					}

				}else{
					if(mode != MODE_STOP){
						mode = MODE_STOP;

						motorDisable();
						serial->printf("\n\rmode = STOP\n\r\n\r");
						led[0]->interval(50);
						led[1]->interval(1000);
						led[2]->interval(1000);
					}
				}
			}else{
				if(mode != MODE_STOP){
					mode = MODE_STOP;
					speakRequest = 23;
					motorDisable();
					serial->printf("\n\rbatt under voltage\n\rmode = STOP\n\r\n\r");
					led[0]->interval(50);
					led[1]->interval(1000);
					led[2]->interval(50);

					battUnderVoltage = BATT_UNDER_LIMIT + BATT_UNDER_HYS;
				}
			}
		}else{
			if(mode != MODE_STOP){
				mode = MODE_STOP;

				motorDisable();
				serial->printf("\n\rsa com lost\n\rmode = STOP\n\r\n\r");
				led[0]->interval(50);
				led[1]->interval(1000);
				led[2]->interval(50);
			}
		}

	}else if(millis() - sbus->lastReceiveTime > SBUS_TIMEOUT_TOLERANCE){		//SBUS受信できていない｡無条件
		if(mode != MODE_TIMEOUT){
			motorDisable();
			serial->printf("\n\rmode = TIME OUT\n\r\n\r");
			led[0]->interval(50);
			led[1]->interval(50);
			led[2]->interval(50);
		}
		mode = MODE_TIMEOUT;
	}

	if(millis() > controlCycleIntervalTime){
		controlCycleIntervalTime += 1;
		controlCycle();
	}

	if(speakRequest > 0){
		sa->write(1,speakRequest);
		speakRequest = -1;
	}else if(speakRequest == -1){
		speakRequest = 0;
		sa->write(1,0);
	}

	if(millis() > saSendTime){
		saSendTime += SA_EN_INTERVAL;
		sa->write(0,motorEN);
	}
}

/*************************************************************************************/

void KawaRobo::displayCycle(){
	if(serial->available()){
		while(serial->available()){
			dispValue++;
			serial->read();
		}
	}

	if(millis() > printTime){												//print周期
		printTime += PRINT_CYCLE;
		if(millis() - sbus->lastReceiveTime < SBUS_TIMEOUT_TOLERANCE){		//S.BUS受信できてる時
			if(getSelectPosition() < -0.9 && selectToggle == 0){
				selectToggle = 1;
				dispValue--;
			}else if(getSelectPosition() > 0.9 && selectToggle == 0){
				selectToggle = 1;
				dispValue++;
			}else{
				selectToggle = 0;
			}
		}


		if(sw[0]->readStat == 1 && sw[0]->read() == 0){
			dispValue++;
			//sa->write(1,dispValue + 1);
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
			serial->printf("v = ,%d,mv,under voltage = %5d,adc2V = %4d",(int)(sa->read(3) * adcToBattV),battUnderVoltage,(int)(adcToBattV*100));
			break;

		case 4:
			serial->printf("getRevPosition = %4d,robotR = ,%4d,targetR = ,%4d,robotPID = ,%4d",(int)(getRevPosition()*100),(int)robotR.outputF(),(int)robotTargetR.outputF(),robotPIDR.output32());
			break;

		case 5:
			serial->printf("getArmPosition = ,%4d",(int)(getArmPosition()*100));
			serial->printf("arm  analog");
			serial->printf("deg = ,%4d,PID out = ,%7d",(int)armDegree,armPID.output32());
			serial->printf("arm PID P = ,%7d,D = ,%4d",(int)(armPID.error*100),(int)(armPID.errorDiv*100));
			serial->printf("armTrg = %4d",(int)armTarget.errorInt);
			break;

		case 6:
			serial->printf("run = %4d,rev = %4d,arm = %4d,",(int)(getRunPosition()*100),(int)(getRevPosition()*100),(int)(getArmPosition()*100));
			serial->printf("motot out ");
			serial->printf(",%4d",(int)(motor[0]->outDuty*100));
			serial->printf(",%4d",(int)(motor[1]->outDuty*100));
			serial->printf(",%4d",(int)(motor[3]->outDuty*100));
			break;

		default:
			dispValue = 1;
		}

		serial->printf("\n\r");
	}
}

void KawaRobo::controlCycle(){		//

	switch(mode){
	case MODE_RUN:
		run();
		break;

	case MODE_TEST:
		motorDisable();
		test();
		break;

	case MODE_STOP:
		motorDisable();

		break;

	}
}

/*************************************************************************************/

void KawaRobo::test(){

}

void KawaRobo::run(){
	if(getTogglePosition(3) <= 0){
		motor[0]->duty(+getRunPosition() + getRevPosition());
		motor[3]->duty(-getRunPosition() + getRevPosition());



		if(armDegree < -110 && getArmPosition() < 0){
			armPID.measuredValue(armDegree + 120);
			armTarget.measuredValue(-getArmPosition());
			motor[1]->duty(armPID.outputF());

		}else if(armDegree > 110 && getArmPosition() > 0){
			armPID.measuredValue(armDegree - 120);
			armTarget.measuredValue(-getArmPosition());
			motor[1]->duty(armPID.outputF());

		}else{
			motor[1]->duty(getArmPosition());
		}


		return;
	}

	robotPIDR.measuredValue(robotR.outputF() - robotTargetR.outputF());
	motor[0]->duty(+getRunPosition() - robotPIDR.outputF());
	motor[3]->duty(-getRunPosition() - robotPIDR.outputF());

	if(getArmPosition() < -0.98){
		armPID.measuredValue(armDegree - ARM_TOP_DEG);	//トップまで一気に上げる
		armTarget.clear();
		armTarget.errorInt = ARM_DEF_DEG;
	}else{
		armPID.measuredValue(armDegree - armTarget.outputF());
		armTarget.measuredValue(-getArmPosition());
	}
	motor[1]->duty(armPID.outputF());

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
}

//制御周期関係なくぶんまわし
void KawaRobo::armPotCycle(){
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

	robotTargetR.measuredValue(getRevPosition());
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

	//motor[1]->dutyLimit(0.3);

	printValueSelect = 0;
	battUnderVoltage = BATT_UNDER_LIMIT;

	dispValue = 1;
	speakRequest = 0;

	saSendTime = millis();
	printTime = millis();

	robotR.setup(0,1,0);			//ジャイロによる角度			単位度
	robotTargetR.setup(0,0.1,0);	//プロポによる角度設定		単位度
	robotPIDR.setup(0.01,0,0);		//制御用コントローラ､ゲイン1で1deg→100%になる
	robotPIDR.outLimit(-1,1);

	armPID.setup(0.01,0,0);		//ポテンショによる位置決め??
	armTarget.setup(0,1,0);		//プロポによる目標角度(？)設定

	armTarget.errorLimitInt(ARM_BTM_DEG,ARM_TOP_DEG);

	armCurrent.setup(0,1,0);
	armCurrent.errorLimitInt(0,100);

	sa->write(2,5);
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
	motorEN = 0;		//motor disable
	for(int i=0;i<4;i++){
		motor[i]->duty(0);
	}

}

void KawaRobo::motorEnable(){
	motorEN = 1;		//motor enable
	for(int i=0;i<4;i++){
		motor[i]->duty(0);
	}
}

/*************************************************************************************/

float KawaRobo::getRunPosition(){
	return (float)getStickPosition(CHANNEL_RUN)/STICK_MAX;
}

float KawaRobo::getArmPosition(){
	if(sbus->read(CHANNEL_ARM) < 802){
		return (float)getStickPosition(CHANNEL_ARM,585)/(STICK_MAX/3);
	}else if(sbus->read(CHANNEL_ARM) < 1242){
		return (float)getStickPosition(CHANNEL_ARM,1024)/(STICK_MAX/3);
	}else{
		return (float)getStickPosition(CHANNEL_ARM,1464)/(STICK_MAX/3);
	}
}

float KawaRobo::getRevPosition(){
	return (float)getStickPosition(CHANNEL_REV)/STICK_MAX;
}

float KawaRobo::getSelectPosition(){
	return (float)getStickPosition(CHANNEL_SEL)/STICK_MAX;
}

float KawaRobo::getTogglePosition(uint16_t num){
	switch(num){
	case 0:
		if(sbus->read(CHANNEL_TOGGLE0) < 805){
			return 1.0;
		}else if(sbus->read(CHANNEL_TOGGLE0) < 1245){
			return 0;
		}else{
			return -1.0;
		}
		break;

	case 1:
		if(sbus->read(CHANNEL_TOGGLE1) < 500){
			return 1.0;
		}else if(sbus->read(CHANNEL_TOGGLE1) < 1500){
			return 0;
		}else{
			return -1.0;
		}
		break;

	case 2:
		if(sbus->read(CHANNEL_TOGGLE2) < 500){
			return 1.0;
		}else if(sbus->read(CHANNEL_TOGGLE2) < 1500){
			return 0;
		}else{
			return -1.0;
		}
		break;
	case 3:
		if(sbus->read(CHANNEL_TOGGLE3) < 500){
			return 1.0;
		}else{
			return -1.0;
		}
		break;

	default:
		return 0;
	}
}

float KawaRobo::getVolumePosition(){
	return (float)(sbus->read(CHANNEL_VOLUME)-1024)/880;
}

int KawaRobo::getStickPosition(uint16_t channel,uint16_t offset){
	int data = sbus->read(channel) - offset;

	if(data >= 0){
		if(data < STICK_TOLERANCE){
			data = 0;
		}else{
			data -= STICK_TOLERANCE;
		}
	}else{
		if(data > -STICK_TOLERANCE){
			data = 0;
		}else{
			data += STICK_TOLERANCE;
		}
	}

	if(data > STICK_MAX)data = STICK_MAX;
	if(data < -STICK_MAX)data = -STICK_MAX;

	return data;
}
