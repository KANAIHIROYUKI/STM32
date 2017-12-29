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

	/**********************���T�C�N���񂷃}����KRM****************************/
	if(millis() > SERIKA_TIME && serika && sa->update){//���Z�b�g�����������ɂȂ顕��ʂɋN�������Arduino���̕����N���x���̂Ŗ�Ȃ��
		serika = 0;
		speakRequest = 63;						//�A�C�h���}�X�^�[�V���f�����K�[���Y
	}

	if(sa->update){
		sa->update = 0;
		saDataUpdate();
	}

	if(sbus->update){														//SBUS��M������{�b�g�̐��������sbus�̎�M�����ˑ�
		sbus->update = 0;
		sbusDataUpdate();

	}else if(millis() - sbus->lastReceiveTime > SBUS_TIMEOUT_TOLERANCE){		//SBUS��M�ł��Ă��Ȃ��������
		if(mode != MODE_TIMEOUT){
			motorDisable();
			serial->printf("\n\rmode = TIME OUT\n\r\n\r");
			led[0]->interval(50);
			led[1]->interval(50);
			led[2]->interval(50);
		}
		mode = MODE_TIMEOUT;
	}

	if(millis() > controlCycleIntervalTime){	//�������
		controlCycleIntervalTime += 1;
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
		case MODE_TIMEOUT:
			motorDisable();

			break;
		}
	}

	if(speakRequest > 0){							//�����v��
		sa->write(1,speakRequest);
		serial->printf("\n\rserika = %d\n\r",speakRequest);
		sa->sendRequest = 0;
		speakRequest = -1;
		sa->sendEnded = 0;
	}else if(speakRequest == -1 && sa->sendEnded == 1){
		speakRequest = 0;
		sa->write(1,0);
		sa->sendRequest = 0;
	}

	if(millis() > saSendTime){						//PowerEN�̑��M�Ԋu(�A���œ����������Arduino���~�܂�)
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

	if(millis() > printTime){												//print����
		printTime += PRINT_CYCLE;
		if(millis() - sbus->lastReceiveTime < SBUS_TIMEOUT_TOLERANCE){		//S.BUS��M�ł��Ă鎞
			if(getSelectPosition() < -0.9 && selectToggle == 0){
				selectToggle = 1;
				dispValue--;
				speakRequest = speakRequestNext;
				speakRequestNext--;
			}else if(getSelectPosition() > 0.9 && selectToggle == 0){
				selectToggle = 1;
				dispValue++;
				speakRequest = speakRequestNext;
				speakRequestNext++;
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
			serial->printf(",g = %d",gravitatyAve.peek());
			break;

		case 2:
			serial->printf("sbus ");
			for(int i=0;i<8;i++){
				serial->printf(",%5d",sbus->read(i));
			}
			serial->printf(",gy = %d",armPitchGy.output32());
			break;

		case 3:
			serial->printf("v = ,%d,mv,under voltage = %5d,adc2V = %4d",(int)battVoltage.peek(),(int)battUnderVoltage,(int)(adcToBattV*100));
			break;

		case 4:
			serial->printf("getRevPosition = %4d,robotR = ,%4d,targetR = ,%4d,robotPID = ,%4d",(int)(getRevPosition()*100),(int)robotR.outputF(),(int)robotTargetR.outputF(),robotPIDR.output32());
			break;

		case 5:
			serial->printf("getArmPosition = ,%4d",(int)(getArmPosition()*100));
			serial->printf("arm  analog");
			serial->printf("deg = ,%4d,PID out = ,%7d",(int)armDegree,armPID.output32());
			serial->printf("arm PID P = ,%7d,D = ,%4d",(int)(armPID.error*100),(int)(armPID.errorDiv*100));
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

/*************************************************************************************/

void KawaRobo::test(){

}

void KawaRobo::run(){
	if(armDegree > 30 && turnoverReturn == 1){	//�]�|���A
		turnoverReturn = 2;
		switch(millis() %2){
		case 0:
			speakRequest = 54;//�悢����
			break;
		case 1:
			speakRequest = 8;//���񂵂�
			break;
		}
	}

	if(getArmPosition() < -0.5 && armDegree > 15 && millis() - armSpeakTime > 500 && turnoverReturn == 0){
		armSpeakTime = millis();
		switch(millis()%7){			//�S����͂��I�
		case 0:
			speakRequest = 41;
			break;
		case 1:
			speakRequest = 40;
			break;
		case 2:
			speakRequest = 39;
			break;
		case 3:
			speakRequest = 42;
			break;
		case 4:
			speakRequest = 43;
			break;
		case 5:
			speakRequest = 12;
			break;
		case 6:
			speakRequest = 13;
			break;
		}
	}else if(armDegree > 30 && millis() - armSpeakTime < 500 && turnoverReturn == 0){
		armSpeakTime = millis();
	}

	float output = getArmPosition() - armPitchGy.outputF();
	if(armDegree < ARM_BTM_DEG && output > 0.1){				//����������ɋ߂Â��Ă����ꍇ�D����Ő���������duty0.1�ł͐������[�h�ɓ��炸��������
		armPID.measuredValue(-armDegree + ARM_BTM_DEG);
		motor[1]->duty(armPID.outputF());

	}else if(armDegree > ARM_TOP_DEG && output < -0.1){
		armPID.measuredValue(-armDegree + ARM_TOP_DEG);
		motor[1]->duty(armPID.outputF());

	}else{
		motor[1]->duty(output);						//����ł������ł��Ȃ��̂Ŋ��S�蓮
	}

	if(getTogglePosition(TOGGLE_CONTROL_MODE) <= 0){				//�蓮���[�h
		motor[0]->duty(+getRunPosition() + getRevPosition());
		motor[3]->duty(-getRunPosition() + getRevPosition());
		return;
	}
																	//���䃂�[�h
	robotTargetR.measuredValue(getRevPosition());
	robotPIDR.measuredValue(robotR.outputF() - robotTargetR.outputF());
	motor[0]->duty(+getRunPosition() - robotPIDR.outputF());
	motor[3]->duty(-getRunPosition() - robotPIDR.outputF());

	//armCurrent.measuredValue((motor[1]->outDuty*14) - 0.1);
}



/*************************************************************************************/

//SerialArduino��M�������̏���
void KawaRobo::saDataUpdate(){
	robotR.measuredValue(-(float)(sa->read(0))/39200);
	if(abs(sa->read(1)) > 100){
		armPitchGy.measuredValue((float)sa->read(1)/39200);
	}

	if(battVoltage.valueCnt > 500){ 			//100�񕽋�
		float val = battVoltage.read();
		battVoltage.reset();

		battVoltage.valueCnt = 10;				//����10�񕪂�O��̕��ςœ����
		battVoltage.stack(val*10);
	}
	battVoltage.stack((sa->read(3) * adcToBattV));

	gravitatyAve.stack(sa->read(8)/10);
	if(gravitatyAve.valueCnt > 50){
		if(gravitatyAve.peek() < -100){		//���Ԃ�������s���Ă����Ԃ�-1500���炢
			if(turnoverReturn == 0){
				switch(millis()%9){
				case 0:
					speakRequest = 11;//�����[��
					break;
				case 1:
					speakRequest = 77;//�p���������ł�
					break;
				case 2:
					speakRequest = 84;//����ꂿ�Ⴂ�܂���
					break;
				case 3:
					speakRequest = 47;//���͂�
					break;
				case 4:
					speakRequest = 48;
					break;
				case 5:
					speakRequest = 49;
					break;
				case 6:
					speakRequest = 50;
					break;
				case 7:
					speakRequest = 51;
					break;
				case 8:
					speakRequest = 46;//���͂�
					break;
				}
				turnoverReturn = 1;
			}
		}else if(gravitatyAve.peek() > 1000 && turnoverReturn != 0){	//���]���ĂȂ���
			switch(millis()%4){//���A������
			case 0:
				speakRequest = 31;//�ł��I
				break;
			case 1:
				speakRequest = 53;//���܂����I
				break;
			case 2:
				speakRequest = 25;//���Ⴖ��[��I
				break;
			case 3:
				speakRequest = 30;//�ł��܂����I
				break;
			}
			turnoverReturn = 0;
		}
		gravitatyAve.read();
	}


}

//S.BUS��M�������̏���
void KawaRobo::sbusDataUpdate(){
	float value = +(float)(((pot1Int/pot1Cnt)-2048)/4096);// - (pot2Int/pot2Cnt))/8191;
	potCnt = pot1Cnt + pot2Cnt;
	pot1Cnt = 0;
	pot1Int = 0;
	pot2Cnt = 0;
	pot2Int = 0;
	armDegree = -value * 270;

	if(millis() - sa->lastReceiveTime < SA_TIMEOUT_TOLERANCE){
		if(battVoltage.peek() > battUnderVoltage){		//Arduino����̎�M�����餂��d��OK
			battUnderVoltage = BATT_UNDER_LIMIT;

			if(getTogglePosition(0) < 0){				//�v���|�̃g�O���X�C�b�`�̏�Ԃɂ���ē��샂�[�h��ύX����
				if(mode != MODE_RUN){
					mode = MODE_RUN;

					speakRequest = 83;					//�킽���ɂ����Ȃ��Ƥ�����ĉ������ˁI


					motorEnable();
					serial->printf("\n\rmode = RUN\n\r\n\r");
					led[0]->interval(1000);
					led[1]->interval(1000);
					led[2]->interval(1000);

					robotPIDR.clear();		//����񓮂����Ȃ��悤��PID�R���g���[���[�����Z�b�g
					robotTargetR.clear();	//�ڕW�p�x��0�ɂ��餌��݂̊p�x�ƖڕW�p�x�̍��𖳂���
					robotR.clear();			//�W���C���p�x�̐ϕ��l�����Z�b�g����݊p�x��0�x�Ƃ���

					armPID.clear();			//����񓮂����Ȃ��悤��PID�R���g���[���[�����Z�b�g
				}

			}else if(getTogglePosition(0) < 0.5){
				if(mode != MODE_TEST){
					mode = MODE_TEST;
					sa->write(1,0);
					speakRequest = 60;		//�킽�������΂�܂���

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
				speakRequest = 28;		//���߂ł���I�I
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

	if(toggleStat[TOGGLE_CONTROL_MODE] != (int)getTogglePosition(TOGGLE_CONTROL_MODE)){		//�蓮������؂�ւ�
		if(getTogglePosition(TOGGLE_CONTROL_MODE) > 0){
			speakRequest = 5;						//�����Ƃ��댩�����Ⴂ�܂��ˁI
		}else{
			speakRequest = 67;						//�ꐶ��������΂�܂��I
		}
		if(toggleStat[TOGGLE_CONTROL_MODE] == 2)speakRequest = 0;							//����ݒ�
		toggleStat[TOGGLE_CONTROL_MODE] = (int)getTogglePosition(TOGGLE_CONTROL_MODE);
	}

	if(toggleStat[TOGGLE_POWER_LIMIT] != (int)getTogglePosition(TOGGLE_POWER_LIMIT)){		//�o�̓��~�b�^�؂�ւ�
		if(getTogglePosition(TOGGLE_POWER_LIMIT) > 0.1){
			motor[3]->dutyLimit(0.95);
			motor[0]->dutyLimit(0.95);
			speakRequest = 36;						//�ǂ�Ȃ��Ƃ����߂��Ⴂ���Ȃ����Ĥ�v���f���[�T�[���񂪍s���Ă܂����I
		}else if(getTogglePosition(TOGGLE_POWER_LIMIT) > -0.1){
			motor[3]->dutyLimit(0.7);
			motor[0]->dutyLimit(0.7);
			speakRequest = 67;						//�ꐶ��������΂�܂��I
		}else{
			motor[3]->dutyLimit(0.4);
			motor[0]->dutyLimit(0.4);
			speakRequest = 66;						//�ꐶ�������܂����I
		}
		if(toggleStat[TOGGLE_POWER_LIMIT] == 2)speakRequest = 0;							//����ݒ�
		toggleStat[TOGGLE_POWER_LIMIT] = (int)getTogglePosition(TOGGLE_POWER_LIMIT);
	}
}

//��������֌W�Ȃ��Ԃ�܂킵
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
}


/***********************************�����ݒ�֐��Q**************************************************/

void KawaRobo::setup(USART &serialSet,SBUS &sbusSet,SerialArduino &saSet,NCP5359 &motor0,NCP5359 &motor1,NCP5359 &motor2,NCP5359 &motor3){
	this->serial = &serialSet;
	this->sbus = &sbusSet;
	this->sa   = &saSet;

	this->motor[0] = &motor0;
	this->motor[1] = &motor1;
	this->motor[2] = &motor2;
	this->motor[3] = &motor3;

	motor[1]->dutyLimit(0.9);
	//motor[2]->dutyLimit(0.2);

	motor[3]->dutyLimit(0.9);
	motor[0]->dutyLimit(0.9);

	printValueSelect = 0;
	battUnderVoltage = BATT_UNDER_LIMIT;

	dispValue = 1;
	speakRequest = 0;
	speakRequestNext = 0;
	serika = 1;
	turnoverReturn = 0;
	armOffsetDuty = 0;

	saSendTime = millis();
	printTime = millis();


	/*
	robotR.setup(0,1,0);			//�W���C���ɂ��p�x			�P�ʓx
	robotTargetR.setup(0,0.1,0);	//�v���|�ɂ��p�x�ݒ�		�P�ʓx
	robotPIDR.setup(0.01,0,0);		//����p�R���g���[����Q�C��1��1deg��100%�ɂȂ�
	robotPIDR.outLimit(-1,1);//*/

	armPitch.setup(0.01,0,0);
	armPitchGy.setup(1,0,0);			//�W���C���ɂ��p�x�ϕ�

	robotR.setup(0.5,0,0);			//�W���C���ɂ�鑬�x			�P�ʓx
	robotTargetR.setup(1,0,0);		//�v���|�ɂ�鑬�x�ݒ�		�P�ʓx
	robotPIDR.setup(1,0,0);		//����p�R���g���[����Q�C��1��1deg��100%�ɂȂ�
	robotPIDR.outLimit(-1,1);

	armPID.setup(0,0,-10);		//�@�\���E�����p

	armCurrent.setup(0,1,0);
	armCurrent.errorLimitInt(0,100);

	toggleStat[TOGGLE_CONTROL_MODE] = 2;
	toggleStat[TOGGLE_POWER_LIMIT]  = 2;
	//cyclle�܂�����Ă˂���
	//toggleStat[TOGGLE_CONTROL_MODE] = (int)getTogglePosition(TOGGLE_CONTROL_MODE);
	//toggleStat[TOGGLE_POWER_LIMIT]  = (int)getTogglePosition(TOGGLE_POWER_LIMIT);
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

void KawaRobo::sensorSetup(ADC &adc0,ADC &adc1,ADC &adc2,ADC &adc3,float _adcToBatt){
	this->analog[0] = &adc0;
	this->analog[1] = &adc1;
	this->analog[2] = &adc2;
	this->analog[3] = &adc3;
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
