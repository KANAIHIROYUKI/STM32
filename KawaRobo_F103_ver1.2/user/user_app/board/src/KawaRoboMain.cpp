#include "KawaRoboMain.h"

void KawaRobo::cycle(){

	sbus->cycle();
	if((error | SA_LOST) == SA_LOST && error){		//Arduino�����N�����Ă��Ȃ����͑��M��~����
		sa->sendRequest = 0;
	}
	sa->cycle();

	for(int i=0;i<6;i++){
		led[i]->cycle();
	}
	for(int i=0;i<4;i++){
		motor[i]->cycle();
	}

	sw[0]->cycle();
	sw[1]->cycle();

	armPotCycle();
	displayCycle();

	/**********************���T�C�N���񂷃}����KRM****************************/
	if(millis() > SERIKA_TIME && serika && sa->update){//���Z�b�g�����������ɂȂ顕��ʂɋN�������Arduino���̕����N���x���̂Ŗ�Ȃ��
		serika = 0;
		speakRequest = 10;						//�A�C�h���}�X�^�[�V���f�����K�[���Y
	}

	if(sa->update){
		sa->update = 0;
		saDataUpdate();
	}

	if(millis() - sbus->lastReceiveTime > SBUS_TIMEOUT_TOLERANCE){		//��M�@�ƒʐM�ł��Ȃ��������
		if(mode != MODE_ERROR){
			motorDisable();
			serial->printf("\n\rmode = ERROR :: SBUS\n\r\n\r");
			led[1]->write(0);
			error |= SBUS_LOST;											//SBUS�ʐM�ł��Ă��Ȃ�
		}
		mode = MODE_ERROR;

	}else if(sbus->frameOk == 0){
		if(mode != MODE_DISCONNECT){
			serial->printf("\n\rmode = DISCONNECT\n\r\n\r");

			led[1]->interval(50);
			error &= 0xFFFF - SBUS_LOST;								//SBUS�ʐM�͂ł��Ă��邪����M�@�Ɩ��ڑ�
		}
		mode = MODE_DISCONNECT;
	}else if(sbus->update){												//SBUS��M��
		sbus->update = 0;
		if(mode == MODE_DISCONNECT || mode == MODE_ERROR){				//����܂Ń^�C���A�E�g�������̂š

			led[1]->write(1);
			error &= 0xFFFF - SBUS_LOST;								//SBUS�ʐMOK����M�@�Ɛڑ�
		}
		sbusDataUpdate();
	}

	if(SA_RECEIVE_OK){			//SerialArduino��LED�ƃG���[����
		if(saLedStat == 0){
			saLedStat = 1;
			led[0]->write(1);
			error &= 0xFFFF - SA_LOST;
		}
	}else{
		if(saLedStat == 1){
			saLedStat = 0;
			led[0]->write(0);
			error |= SA_LOST;
		}
	}

	if(error){
		if(error != oldError){
			if(error == Batt_UVL){	//�����d���̂�
				led[3]->interval(1000);
			}else{
				led[3]->write(1);
			}

			oldError = error;
		}
	}else if(error != oldError){	//�G���[�Ȃ��Ȃ���
		oldError = error;
		led[3]->write(0);
	}

	if(millis() > controlCycleIntervalTime){	//�������
		controlCycleIntervalTime += 1;
		if(mode == MODE_RUN || mode == MODE_TEST){
			run();
		}else if(mode == MODE_STOP || mode == MODE_DISCONNECT || mode == MODE_ERROR){
			safety();
		}
	}

	if(speakRequest > 0){							//�����v��
		sa->write(1,speakRequest);
		serial->printf("\n\rserika = %d\n\r",speakRequest);
		sa->sendRequest = 0;
		speakRequest = -1;
		sa->sendEnded = 0;

		loopCycleCnt++;

	}else if(speakRequest == -1 && sa->sendEnded == 1){
		speakRequest = 0;
		sa->write(1,0);
		sa->write(2,0);
		sa->sendRequest = 0;
	}

	if(millis() > saSendTime){						//PowerEN�̑��M�Ԋu(�A���œ����������Arduino���~�܂�)
		saSendTime += 50;
		sa->write(0,0);
	}
}

/*************************************************************************************/

void KawaRobo::displayCycle(){
	if(serial->available()){
		while(serial->available()){
			if(serial->peek() == 'p'){
				robotRGain *= 1.1;
				robotR.setup(robotRGain,0,0);
				serial->printf("\n\r\n\rr gain = %d\n\r\n\r",(int)(robotRGain*100));
			}else if(serial->peek() == 'm'){
				robotRGain *= 0.9;
				robotR.setup(robotRGain,0,0);
				serial->printf("\n\r\n\rr gain = %d\n\r\n\r",(int)(robotRGain*100));
			}else if(serial->peek() == 'o'){
				armPitchGain *= 1.1;
				armPitchGy.setup(armPitchGain,0,0);
				serial->printf("\n\r\n\rarm gain = %d\n\r\n\r",(int)(armPitchGain*100));
			}else if(serial->peek() == 'n'){
				armPitchGain *= 0.9;
				armPitchGy.setup(armPitchGain,0,0);
				serial->printf("\n\r\n\rarm gain = %d\n\r\n\r",(int)(armPitchGain*100));
			}{
				dispValue++;
			}
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
			//sa->write(2,30);
			//sa->write(1,dispValue + 1);
		}

		if(sw[1]->readStat == 1 && sw[1]->read() == 0){
			dispValue--;
			//sa->write(2,15);
		}

		serial->printf("er = %d,",error);
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
			if(sbus->connect){
				serial->printf("connect");
			}else{
				serial->printf("lost");
			}
			if(sbus->frameOk){
				serial->printf("ok");
			}else{
				serial->printf("er");
			}
			for(int i=0;i<8;i++){
				//serial->printf("%d,",sbus->buffer[i]);
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
			serial->printf(",L,%4d",(int)(motor[ML]->outDuty*100));
			serial->printf(",A,%4d",(int)(motor[MA]->outDuty*100));
			serial->printf(",R,%4d",(int)(motor[MR]->outDuty*100));
			break;

		default:
			dispValue = 1;
		}

		serial->printf("\n\r");
	}
}

/*************************************************************************************/

void KawaRobo::run(){
	float value = +(float)(((pot1Int/pot1Cnt)-2048)/4096);// - (pot2Int/pot2Cnt))/8191;
	potCnt = pot1Cnt + pot2Cnt;
	pot1Cnt = 0;
	pot1Int = 0;
	pot2Cnt = 0;
	pot2Int = 0;
	armDegree = -value * 270;

	if(armDegree > 30 && turnoverReturn == 1){	//�]�|���A
		turnoverReturn = 2;
		switch(loopCycleCnt %3){
		case 0:
			speakRequest = 57;//�悢����
			break;
		case 1:
			speakRequest = 47;//���񂵂�
			break;
		case 2:
			speakRequest = 43;//�W���[���v�I
			break;
		}
	}

	if(getArmPosition() < -0.5 && armDegree > 15 && millis() - armSpeakTime > 500 && turnoverReturn == 0){
		armSpeakTime = millis();
		switch(loopCycleCnt%8){			//��͂��I�
		case 0:
			speakRequest = 111;
			break;
		case 1:
			speakRequest = 99;
			break;
		case 2:
			speakRequest = 100;
			break;
		case 3:
			speakRequest = 102;
			break;
		case 4:
			speakRequest = 105;
			break;
		case 5:
			speakRequest = 106;
			break;
		case 6:
			speakRequest = 107;
			break;
		case 7:
			speakRequest = 110;
			break;
		}
	}else if(armDegree > 30 && millis() - armSpeakTime < 500 && turnoverReturn == 0){
		armSpeakTime = millis();
	}

	output = 0;
	if(mode == MODE_TEST){
		output = getArmPosition();								//�W���C���␳�Ȃ�
	}else{
		output = getArmPosition();// + armPitchGy.outputF();		//�W���C���␳����
	}

	if(armDegree < ARM_BTM_DEG && output > 0.1){				//����������ɋ߂Â��Ă����ꍇ�D����Ő���������duty0.1�ł͐������[�h�ɓ��炸��������
		armPID.measuredValue(-armDegree + ARM_BTM_DEG);
		output = armPID.outputF();
		motor[MA]->duty(output);						//
		motor[MS]->duty(output);

	}else if(armDegree > ARM_TOP_DEG && output < -0.1){
		armPID.measuredValue(-armDegree + ARM_TOP_DEG);
		output = armPID.outputF();
		motor[MA]->duty(output);
		motor[MS]->duty(output);

	}else{
		motor[MA]->duty(output);						//����ł������ł��Ȃ��̂Ŋ��S�蓮
		motor[MS]->duty(output);

	}
	/*
	if(millis() - modeChangeTime < 500 && modeChangeTime != 0){
		serial->printf("\n\r%d",(int)(output*100));
	}*/

	if(mode == MODE_TEST){				//�蓮���[�h
		motor[MR]->duty(+getRunPosition() + getRevPosition());
		motor[ML]->duty(-getRunPosition() + getRevPosition());
		return;
	}
																	//���䃂�[�h
	robotTargetR.measuredValue(getRevPosition());
	robotPIDR.measuredValue(robotR.outputF() - robotTargetR.outputF());
	motor[MR]->duty(+getRunPosition() - robotPIDR.outputF());
	motor[ML]->duty(-getRunPosition() - robotPIDR.outputF());

	//armCurrent.measuredValue((motor[1]->outDuty*14) - 0.1);
}

void KawaRobo::safety(){
	float value = +(float)(((pot1Int/pot1Cnt)-2048)/4096);// - (pot2Int/pot2Cnt))/8191;
	potCnt = pot1Cnt + pot2Cnt;
	pot1Cnt = 0;
	pot1Int = 0;
	pot2Cnt = 0;
	pot2Int = 0;
	armDegree = -value * 270;

	armPID.measuredValue(-armDegree + ARM_BTM_DEG);
	robotTargetR.measuredValue(getRevPosition());
	robotPIDR.measuredValue(robotR.outputF() - robotTargetR.outputF());

	motorDisable();
}



/*************************************************************************************/

//SerialArduino��M�������̏���
void KawaRobo::saDataUpdate(){
	robotR.measuredValue(-(float)(sa->read(2))/39200);
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

	gravitatyAve.stack(sa->read(10)/10);
	if(gravitatyAve.valueCnt > 50){
		if(gravitatyAve.peek() < -1000){		//���Ԃ�������s���Ă����Ԃ�-1500���炢
			if(turnoverReturn == 0){
				switch(loopCycleCnt%9){
				case 0:
					speakRequest = 58;//�����[��
					break;
				case 1:
					speakRequest = 49;//�p���������ł�
					break;
				case 2:
					speakRequest = 54;//����ꂿ�Ⴂ�܂���
					break;
				case 3:
					speakRequest = 94;//���͂�
					break;
				case 4:
					speakRequest = 96;
					break;
				case 5:
					speakRequest = 98;
					break;
				case 6:
					speakRequest = 101;
					break;
				case 7:
					speakRequest = 109;
					break;
				case 8:
					speakRequest = 112;//���͂�
					break;
				}
				turnoverReturn = 1;
			}
		}else if(gravitatyAve.peek() > -100 && turnoverReturn != 0){	//���]���ĂȂ���
			switch(loopCycleCnt%4){//���A������
			case 0:
				speakRequest = 103;//�ł��I
				break;
			case 1:
				speakRequest = 76;//���܂����I
				break;
			case 2:
				speakRequest = 70;//���Ⴖ��[��I
				break;
			case 3:
				speakRequest = 46;//�ł��܂����I
				break;//38?
			}
			turnoverReturn = 0;
		}
		gravitatyAve.read();
	}


}

//S.BUS��M�������̏���
void KawaRobo::sbusDataUpdate(){
	//serial->printf("%d\n\r",sbus->read(2));
	if(SA_RECEIVE_OK){
		if(battVoltage.peek() > battUnderVoltage){		//Arduino����̎�M�����餂��d��OK
			battUnderVoltage = BATT_UNDER_LIMIT;
			error &= 0xFFFF - Batt_UVL;
			if(getTogglePosition(TOGGLE_CONTROL_MODE) > 0.5){				//�v���|�̃g�O���X�C�b�`�̏�Ԃɂ���ē��샂�[�h��ύX����
				if(mode != MODE_RUN){

					mode = MODE_RUN;

					switch(loopCycleCnt % 5){
					case 0:
						speakRequest = 18;				//�킽���ɂ����Ȃ��Ƥ�����ĉ������ˁI
						break;
					case 1:
						speakRequest = 14;				//�ꐶ�������K�����̂ŉ������Ă���������
						break;
					case 2:
						speakRequest = 36;				//�����Ƃ��댩�����Ⴂ�܂���
						break;
					case 3:
						speakRequest = 30;				//�킽�������Ă݂����ł�
						break;
					case 4:
						speakRequest = 52;				//��낵�����肢���܂��ˁI
						break;
					}


					motorEnable();
					serial->printf("\n\rmode = RUN\n\r\n\r");
				}

			}else if(getTogglePosition(TOGGLE_CONTROL_MODE) < 0.5){
				if(mode != MODE_TEST){

					if(mode == MODE_RUN){		//RUN��TEST
						switch((int)getTogglePosition(TOGGLE_SITUATION)){
						case -1:				//����
							switch(loopCycleCnt % 5){
							case 0:
								speakRequest = 42;			//���肪�Ƃ��������܂����I
								break;
							case 1:
								speakRequest = 44;			//�Ƃ��Ă��y���������ł��I
								break;
							case 2:
								speakRequest = 55;			//���肪�Ƃ��������܂��I
								break;
							case 3:
								speakRequest = 60;			//���肪�Ƃ��������܂��I
								break;
							case 4:
								speakRequest = 82;			//�������ł��I
								break;
							}
							break;
						case 0:				//����ȊO��d�؂蒼����z�肵�ĉ�������Ȃ�

							break;
						case 1:				//����
							switch(loopCycleCnt % 4){
							case 0:
								speakRequest = 81;			//�������ł��I
								break;
							case 1:
								speakRequest = 17;			//���̤�Ƃ��Ă����ɂł��܂�����ˁI
								break;
							case 2:
								speakRequest = 26;			//�킽����Ƃ��Ă����ꂵ���ł��I
								break;
							case 3:
								speakRequest = 67;			//������[�I
								break;
							}
							break;
						}
					}else{						//STOP��TEST
						switch(loopCycleCnt % 4){
						case 0:
							speakRequest = 79;				//�ǂ��ł����H
							break;
						case 1:
							speakRequest = 89;				//���ւ�
							break;
						case 2:
							speakRequest = 80;				//�����ւ�
							break;
						case 3:
							speakRequest = 45;				//���d���̌��ł��I
							break;
						}
					}

					mode = MODE_TEST;

					motorEnable();
					serial->printf("\n\rmode = TEST\n\r\n\r");
				}

			}else{
				if(mode != MODE_STOP){
					mode = MODE_STOP;

					if(mode == MODE_TEST || mode == MODE_RUN){
						switch(millis() % 4){
						case 0:
							speakRequest = 20;			//�킽���̂��Ƥ�J�߂Ă���܂����H
							break;
						case 1:
							speakRequest = 68;			//�����l�ł��I
							break;
						case 2:
							speakRequest = 86;			//���̂����ł�
							break;
						case 3:
							speakRequest = 15;			//�͂��`��Ȃ�Ƃ��ł��܂���
							break;
						}
					}

					motorDisable();
					serial->printf("\n\rmode = STOP\n\r\n\r");
				}
			}
		}else{
			if(mode != MODE_STOP){						//�d���s��

				if(loopCycleCnt%2){
					speakRequest = 13;//�{�����Ⴂ�܂���
				}else{
					speakRequest = 73;//���߂ł���I
				}
				motorDisable();
				serial->printf("\n\rbatt under voltage\n\rmode = STOP\n\r\n\r");

				error |= Batt_UVL;
				battUnderVoltage = BATT_UNDER_LIMIT + BATT_UNDER_HYS;
			}
		}
	}else{
		if(mode != MODE_ERROR){
			mode = MODE_ERROR;

			motorDisable();

			serial->printf("\n\rmode = ERROR :: SerialArduino\n\r\n\r");
		}
	}

	if(toggleStat[TOGGLE_POWER_LIMIT] != (int)getTogglePosition(TOGGLE_POWER_LIMIT)){		//�o�̓��~�b�^�؂�ւ�
		if(getTogglePosition(TOGGLE_POWER_LIMIT) > 0.1){
			motor[MR]->dutyLimit(0.99);
			motor[ML]->dutyLimit(0.99);
			switch(loopCycleCnt % 2){
			case 0:
				speakRequest = 35;						//�ꐶ��������΂�܂��I
				break;
			case 1:
				speakRequest = 5;						//�ǂ�Ȃ��Ƃ����߂��Ⴂ���Ȃ����Ĥ�v���f���[�T�[���񂪍s���Ă܂����I
				break;
			}
		}else if(getTogglePosition(TOGGLE_POWER_LIMIT) > -0.1){
			motor[MR]->dutyLimit(0.7);
			motor[ML]->dutyLimit(0.7);
			switch(loopCycleCnt % 2){
			case 0:
				speakRequest = 11;						//�������Ă��炤��,�Ȃ�ł��ł����Ⴄ�C�����܂��I
				break;
			case 1:
				speakRequest = 21;						//�����������[�I
				break;
			case 2:
				speakRequest = 56;						//�t�@�C�g�`�I
				break;
			}

		}else{
			motor[MR]->dutyLimit(0.4);
			motor[ML]->dutyLimit(0.4);
			switch(loopCycleCnt % 3){
			case 0:
				speakRequest = 39;						//�ꐶ�������܂����I
				break;
			case 1:
				speakRequest = 15;						//�͂��`��Ȃ�Ƃ��ł��܂���
				break;
			case 2:
				speakRequest = 33;						//�킽�������΂�܂����I
				break;
			}
		}


		if(toggleStat[TOGGLE_POWER_LIMIT] == 2)speakRequest = 0;							//����ݒ�
		toggleStat[TOGGLE_POWER_LIMIT] = (int)getTogglePosition(TOGGLE_POWER_LIMIT);
	}

	if(toggleStat[TOGGLE_VOICE] != (int)getTogglePosition(TOGGLE_VOICE)){
		toggleStat[TOGGLE_VOICE] = (int)getTogglePosition(TOGGLE_VOICE);

		switch((int)getTogglePosition(TOGGLE_SITUATION)){
		case -1:				//�s��
			switch(loopCycleCnt % 2){
			case 0:
				speakRequest = 28;			//���͂́I��������������I
				break;
			case 1:
				speakRequest = 83;			//�����[���I
				break;
			}
			break;
		case 0:				//��
			switch(loopCycleCnt % 3){
			case 0:
				speakRequest = 9;			//���́`��ǂ�����ĐH�ׂ���ǂ���ł��傤��
				break;
			case 1:
				speakRequest = 25;			//�����҂傱�҂傱�����Ⴂ�܂�
				break;
			case 2:
				speakRequest = 16;			//�����ƌ��Ă�Ɩ����Ȃ����Ⴂ����
				break;
			}
			break;
		case 1:				//�D��
			switch(loopCycleCnt % 2){
			case 0:
				speakRequest = 72;			//���߂��Ⴂ�܂��ˁI
				break;
			case 1:
				speakRequest = 19;			//����Ɍ������Ĥ���[����I
				break;
			}
			break;
		}
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

void KawaRobo::setup(USART &serialSet,SBUS &sbusSet,SerialArduino &saSet,NCP5359 &motor0,NCP5359 &motor1,NCP5359 &motor2,NCP5359 &motor3,GPIO &motorEN){
	this->serial = &serialSet;
	this->sbus = &sbusSet;
	this->sa   = &saSet;

	this->motor[0] = &motor0;
	this->motor[1] = &motor1;
	this->motor[2] = &motor2;
	this->motor[3] = &motor3;
	this->motorEn = &motorEN;

	motor[MA]->dutyLimit(0.99);
	motor[MS]->dutyLimit(0.99);
	motor[ML]->dutyLimit(0.4);
	motor[MR]->dutyLimit(0.4);

	printValueSelect = 0;
	battUnderVoltage = BATT_UNDER_LIMIT;

	dispValue = 1;
	speakRequest = 0;
	speakRequestNext = 0;
	serika = 1;
	loopCycleCnt = 0;


	turnoverReturn = 0;
	armOffsetDuty = 0;
	saLedStat = 0;

	saSendTime = millis();
	printTime = millis();

	mode = MODE_DISCONNECT;
	error = NONE;
	/*
	robotR.setup(0,1,0);			//�W���C���ɂ��p�x			�P�ʓx
	robotTargetR.setup(0,0.1,0);	//�v���|�ɂ��p�x�ݒ�		�P�ʓx
	robotPIDR.setup(0.01,0,0);		//����p�R���g���[����Q�C��1��1deg��100%�ɂȂ�
	robotPIDR.outLimit(-1,1);//*/


	armPitchGain = 1;
	armPitchGy.setup(armPitchGain,0,0);			//�W���C���ɂ��p�x�ϕ�

	robotRGain = 0.5;
	robotR.setup(robotRGain,0,0);			//�W���C���ɂ�鑬�x			�P�ʓx
	robotTargetR.setup(1,0,0);		//�v���|�ɂ�鑬�x�ݒ�		�P�ʓx
	robotPIDR.setup(1,0,0);		//����p�R���g���[����Q�C��1��1deg��100%�ɂȂ�
	robotPIDR.outLimit(-1,1);

	armPID.setup(0,0,-10);		//�@�\���E�����p
	armPID.outLimit(-0.1,0.1);

	armCurrent.setup(0,1,0);
	armCurrent.errorLimitInt(0,100);

	toggleStat[TOGGLE_CONTROL_MODE] = 2;
	toggleStat[TOGGLE_POWER_LIMIT]  = 2;
	//cyclle�܂�����Ă˂���
	//toggleStat[TOGGLE_CONTROL_MODE] = (int)getTogglePosition(TOGGLE_CONTROL_MODE);
	//toggleStat[TOGGLE_POWER_LIMIT]  = (int)getTogglePosition(TOGGLE_POWER_LIMIT);
}

void KawaRobo::uiSetup(Switch &sw0,Switch &sw1,LED &led0,LED &led1,LED &led2,LED &led3,LED &led4,LED &led5){
	this->sw[0] = &sw0;
	this->sw[1] = &sw1;

	this->led[0] = &led0;
	this->led[1] = &led1;
	this->led[2] = &led2;
	this->led[3] = &led3;
	this->led[4] = &led4;
	this->led[5] = &led5;
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

	motorEn->write(0);		//motor disable
	led[2]->write(0);
	for(int i=0;i<4;i++){
		motor[i]->duty(0);
	}
}

void KawaRobo::motorEnable(){

	for(int i=0;i<4;i++){
		motor[i]->duty(0);
	}
	motorEn->write(1);		//motor enable
	led[2]->write(1);

	return;
	robotPIDR.clear();		//����񓮂����Ȃ��悤��PID�R���g���[���[�����Z�b�g
	robotTargetR.clear();	//�ڕW�p�x��0�ɂ��餌��݂̊p�x�ƖڕW�p�x�̍��𖳂���
	robotR.clear();			//�W���C���p�x�̐ϕ��l�����Z�b�g����݊p�x��0�x�Ƃ���

	armPID.clear();			//����񓮂����Ȃ��悤��PID�R���g���[���[�����Z�b�g
	armPitchGy.clear();
}

/*************************************************************************************/

float KawaRobo::getRunPosition(){
	return (float)getStickPosition(CHANNEL_RUN)/STICK_MAX;
}

float KawaRobo::getArmPosition(){
	return (float)getStickPosition(CHANNEL_ARM)/(STICK_MAX);

	/*if(sbus->read(CHANNEL_ARM) < 802){

	}else if(sbus->read(CHANNEL_ARM) < 1242){
		return (float)getStickPosition(CHANNEL_ARM,1024)/(STICK_MAX/3);
	}else{
		return (float)getStickPosition(CHANNEL_ARM,1464)/(STICK_MAX/3);
	}*/
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
		if(sbus->read(CHANNEL_TOGGLE0) < 500){
			return 1.0;
		}else if(sbus->read(CHANNEL_TOGGLE0) < 1500){
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
	return 0;
	//return (float)(sbus->read(CHANNEL_VOLUME)-1024)/880;
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
