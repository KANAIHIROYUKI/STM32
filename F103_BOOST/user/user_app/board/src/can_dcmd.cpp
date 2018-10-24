#include "can_dcmd.h"

void CanDCMD::canmdSetup(CanNodeMotorDriver &_md0,CanNodeMotorDriver &_md1,uint16_t buzzerBeepOrder){
	this->motor[0] = &_md0;
	this->motor[1] = &_md1;
	motor[0]->motor->free();
	motor[1]->motor->free();

	motor[0]->motorOverRide(1);
	motor[1]->motorOverRide(1);		//CanNodeMotorDriver�Ƃ��Ă̏o�͖���

	driveStat  = DS_NoPower;
	driveError = DE_None;

	onetimeTrigger = 0;

	overCurrentLimit[0] = DriveLimitCurrent;
	overCurrentLimit[1] = DriveLimitCurrent;

	overCurrentLimit[0] = DriveLimitCurrent * 0.8;
	overCurrentLimit[1] = DriveLimitCurrent * 0.8;

	adcCycleTrigger = 0;

	hardwareOption = 1;

	buzzerDelay = buzzerBeepOrder*100;
}


void CanDCMD::adcSetup(SI8900 &isoSet){
	this->adc = &isoSet;
}

void CanDCMD::emgSetup(CanNodeEmg  &emgSet){
	this->emg = &emgSet;
}

void CanDCMD::optionSetup(GPIO &optionSet){
	this->option = &optionSet;
	if(option->read()){
		hardwareOption = 2;
	}else{
		hardwareOption = 1;
	}
}

/**************************************Setup**********************************************/


void CanDCMD::cycle(){
	motor[0]->cycle();
	motor[1]->cycle();
	adc->cycle();

	cycleFunction();
}


void CanDCMD::cycleFunction(){
	currentRread(ChannelCurrentA);	//print�p�̃f�[�^��Ave�Max�Min��邽�߂ɓǂ܂Ȃ��Ă���
	currentRread(ChannelCurrentB);
	vbattRead();

	if((adc->setupStat == 0 || millis() - adc->receiveTime[0] > 10) && driveStat != DS_NoPower){	//�p���[�Ȃ�(�p���[�n�ɓd�����Ȃ����adc.setupStat = 0�ɂȂ�)�̂�NoPower�ȊO�ɓ����Ă�����

		if(motor[0]->motor->outEnable == 2 || motor[1]->motor->outEnable == 2){
			motor[0]->motor->buzzerStop();
			motor[1]->motor->buzzerStop();
		}

		motor[0]->motor->free();
		motor[1]->motor->free();		//�t���[(�o�͂��ׂăI�t)

		motor[0]->motor->outEnable = 0;
		motor[1]->motor->outEnable = 0;

		motor[0]->ledOverRide(0);
		motor[1]->ledOverRide(0);					//CanNodeMotorDriver��led���삷��K�v�͂Ȃ�

		motor[0]->motorOverRide(1);
		motor[1]->motorOverRide(1);					//motor�N���X��outEnable��0�ɂ��Ă���̂Ťfree�ȊO�ĂׂȂ����Ǥ�ꉞ


		driveStat = DS_NoPower;

		return;
	}

	if(adc->readStat[2]){
		adcCycleTrigger = 1;	//adc�S���ǂݏI�����(�H)�@������܂���
	}

	switch(driveStat){
	case DS_NoPower:
		if(adc->setupStat){
			driveStat = DS_PowerIn;
			driveStatTimer = millis();

			vAve.reset();
			vMin.read();

			cAve[0].reset();
			cAve[1].reset();

			cMax[0].read();
			cMax[1].read();

			currentValue[0] = 0;
			currentValue[1] = 0;
			voltageValue	= 0;
		}

		break;

	case DS_PowerIn:


		//if(millis() - driveStatTimer > SetupDerayTime)							driveError |= DE_UnderVoltage;	//�d�����Ȃ񂩕n��

		if(vbattRead() <= setupvoltageValue && adc->readStat[ChannelVoltage] && millis() - driveStatTimer > 100){	//�d���d���̏㏸���~�܂����玟�̏�Ԃ�
			driveStatTimer = millis();
			driveStat = DS_LowOn;

			voltageValue = 0;
			currentValue[0] = 0;
			currentValue[1] = 0;
		}else{
			setupvoltageValue = vbattRead();
		}

		if(errorTask(driveError))break;
		break;

	case DS_LowOn:

		if(currentRread(ChannelCurrentA) > SetupLimitCurrent && adc->readStat[ChannelCurrentA])driveError |= DE_BreakFEToutAHigh;
		if(currentRread(ChannelCurrentB) > SetupLimitCurrent && adc->readStat[ChannelCurrentB])driveError |= DE_BreakFEToutBHigh;

		//currentRread()

		if(vbattRead() 	 < SetupLimitVoltage	&& adc->readStat[ChannelVoltage])				driveError |= DE_UnderVoltage;
		if(vbattRead()   > SetupOverVoltage		&& adc->readStat[ChannelVoltage])				driveError |= DE_OverVoltage;

		if(errorTask(driveError))break;																//�G���[�Ȃ炷��������

		if(millis() - driveStatTimer > 100){
			motor[0]->motor->pwm1->dutyF(0.5);
			motor[1]->motor->pwm1->dutyF(0.5);

			driveStatTimer = millis();
			driveStat = DS_HighOn;
		}

		break;
	case DS_HighOn:

		if(currentRread(ChannelCurrentA) > SetupLimitCurrent && adc->readStat[ChannelCurrentA])driveError |= DE_BreakFEToutALow;
		if(currentRread(ChannelCurrentB) > SetupLimitCurrent && adc->readStat[ChannelCurrentB])driveError |= DE_BreakFEToutBLow;
		if(vbattRead() 	 < SetupLimitVoltage	&& adc->readStat[ChannelVoltage] )				driveError |= DE_UnderVoltage;

		if(errorTask(driveError))break;																//�G���[�Ȃ炷��������

		if(millis() - driveStatTimer > 100){

			motor[0]->led->write(0);
			motor[1]->led->write(0);

			motor[0]->motor->outEnable = 1;
			motor[1]->motor->outEnable = 1;	//�u�U�[�g�����߂Ƀ��[�^�[�o�͗L��

			motor[0]->motor->buzzerStart(BuzzerFrq,0.1);
			motor[1]->motor->buzzerStart(BuzzerFrq,0.1);	//�u�U�[(50ms)

			buzzerTest[0] = 0;
			buzzerTest[1] = 0;

			driveStatTimer = millis();
			driveStat = DS_MotorBuzzerCurrentTest;
		}

		break;
	case DS_MotorBuzzerCurrentTest:

		if(currentRread(ChannelCurrentA) > BuzzerLimitCurrent && adc->readStat[ChannelCurrentA])driveError |= DE_OCoutA;		//�u�U�[���[�h��������ĕی�͂���
		if(currentRread(ChannelCurrentB) > BuzzerLimitCurrent && adc->readStat[ChannelCurrentB])driveError |= DE_OCoutB;
		if(vbattRead() 	 < SetupLimitVoltage	&& adc->readStat[ChannelVoltage])				driveError |= DE_UnderVoltage;

		if(currentRread(ChannelCurrentA) > buzzerTest[ChannelCurrentA])buzzerTest[ChannelCurrentA] = currentRread(ChannelCurrentA);
		if(currentRread(ChannelCurrentB) > buzzerTest[ChannelCurrentB])buzzerTest[ChannelCurrentB] = currentRread(ChannelCurrentB);

		if(millis() - driveStatTimer > 20){
			motor[0]->motor->buzzerStart(BuzzerFrq,0);
			motor[1]->motor->buzzerStart(BuzzerFrq,0);		//�u�U�[OFF

			buzzerTest[ChannelCurrentA] = 0.1*BuzzerTargetCurrent/buzzerTest[ChannelCurrentA];
			buzzerTest[ChannelCurrentB] = 0.1*BuzzerTargetCurrent/buzzerTest[ChannelCurrentB];

			if(buzzerTest[ChannelCurrentA] >= 0.5)buzzerTest[ChannelCurrentA] = 0.5;
			if(buzzerTest[ChannelCurrentB] >= 0.5)buzzerTest[ChannelCurrentB] = 0.5;

			driveStatTimer = millis();
			driveStat = DS_MotorBuzzerDelay;
		}

		break;
	case DS_MotorBuzzerDelay:

		if(currentRread(ChannelCurrentA) > BuzzerLimitCurrent && adc->readStat[ChannelCurrentA])driveError |= DE_OCoutA;		//�u�U�[���[�h��������ĕی�͂���
		if(currentRread(ChannelCurrentB) > BuzzerLimitCurrent && adc->readStat[ChannelCurrentB])driveError |= DE_OCoutB;
		if(adc->readStat[ChannelVoltage] && vbattRead() 	 < SetupLimitVoltage)				driveError |= DE_UnderVoltage;

		if(errorTask(driveError))break;																//�G���[�Ȃ炷��������

		if(millis() - driveStatTimer > buzzerDelay){
			motor[0]->motor->buzzerStart(BuzzerFrq,buzzerTest[ChannelCurrentA]);
			motor[1]->motor->buzzerStart(BuzzerFrq,buzzerTest[ChannelCurrentB]);	//�u�U�[(500ms)

			buzzerTest[ChannelCurrentA] = 0;
			buzzerTest[ChannelCurrentB] = 0;

			driveStatTimer = millis();
			driveStat = DS_MotorBuzzer;
		}

		break;
	case DS_MotorBuzzer:

		if(currentRread(ChannelCurrentA) > BuzzerLimitCurrent && adc->readStat[ChannelCurrentA])driveError |= DE_OCoutA;
		if(currentRread(ChannelCurrentB) > BuzzerLimitCurrent && adc->readStat[ChannelCurrentB])driveError |= DE_OCoutB;
		if(vbattRead() 	 < SetupLimitVoltage	&& adc->readStat[ChannelVoltage])				driveError |= DE_UnderVoltage;

		if(errorTask(driveError))break;																//�G���[�Ȃ炷��������

		if(currentRread(ChannelCurrentA) > buzzerTest[ChannelCurrentA])buzzerTest[ChannelCurrentA] = currentRread(ChannelCurrentA);
		if(currentRread(ChannelCurrentB) > buzzerTest[ChannelCurrentB])buzzerTest[ChannelCurrentB] = currentRread(ChannelCurrentB);

		if(millis() - driveStatTimer > 200){

			motor[0]->motor->buzzerStop();
			motor[1]->motor->buzzerStop();	//�u�U�[�����܂�

			motor[0]->motorOverRide(0);
			motor[1]->motorOverRide(0);				//canNodeMotorDriver�̃��[�^�[�o�͊֐��Ăяo�����L����

			motor[0]->motor->free();
			motor[1]->motor->free();			//�Ƃ肠����������

			motor[0]->ledOverRide(0);
			motor[1]->ledOverRide(0);				//led�����CanNodeMotorDriver�ɕԂ�

			//underVoltageLimitAve = vbattRead()*0.8;

			driveStatTimer = millis();

			driveStat = DS_Drive;

			vAve.reset();
			vMin.read();

			cAve[0].reset();
			cAve[1].reset();

			cMax[0].read();
			cMax[1].read();
		}

		break;
	case DS_Drive:

		if(currentRread(ChannelCurrentA) > overCurrentLimit[ChannelCurrentA] && adc->readStat[ChannelCurrentA])	driveError |= DE_OCoutA;
		if(currentRread(ChannelCurrentB) > overCurrentLimit[ChannelCurrentB] && adc->readStat[ChannelCurrentB])	driveError |= DE_OCoutB;
		if(vbattRead() 	 < DriveLimitVoltage	&& adc->readStat[ChannelVoltage])								driveError |= DE_UnderVoltage;

		if(millis() - driveStatTimer >= DriveIntervalTime){
			driveStatTimer = millis();

			printcAve[0] = cAve[0].read();
			printcAve[1] = cAve[1].read();

			printvAve	 = vAve.read();
			printvMin	 = vMin.read();

			printcMax[0] = cMax[0].read();
			printcMax[1] = cMax[1].read();

			if(printcAve[ChannelCurrentA] > overCurrentLimitAve[ChannelCurrentA])	driveError |= DE_AveOCoutA;
			if(printcAve[ChannelCurrentB] > overCurrentLimitAve[ChannelCurrentB])	driveError |= DE_AveOCoutB;
		}

		if(errorTask(driveError))break;

		break;
	case DS_Error:

		if(millis() - driveStatTimer >= DriveIntervalTime){
			driveStatTimer = millis();
			printcAve[0] = cAve[0].read();
			printcAve[1] = cAve[1].read();
			printvAve	 = vAve.read();

			printvMin	 = vMin.read();
			printcMax[0] = cMax[0].read();
			printcMax[1] = cMax[1].read();

			//if(printcAve[ChannelCurrentA] > overCurrentLimitAve[ChannelCurrentA])	driveError |= DE_AveOCoutB;
			//if(printcAve[ChannelCurrentB] > overCurrentLimitAve[ChannelCurrentB])	driveError |= DE_AveOCoutB;
		}

		break;

	default:
		driveError = DE_Unknown;
		if(errorTask(driveError))break;

		break;
	}
}

/*********************************���펞�Ăяo���@���K�X�Ăяo��*****************************************/

void CanDCMD::overCurrentPeakSet(uint16_t channel,float current_A){
	if(channel > 1)return;
	overCurrentLimit[channel] = current_A;
}

void CanDCMD::overCurrentAveSet(uint16_t channel,float current_A){
	if(channel > 1)return;
	overCurrentLimitAve[channel] = current_A;
}

uint16_t CanDCMD::powerInOnetime(){
	if(onetimeTrigger){
		onetimeTrigger = 0;
		return 1;
	}
	return 0;
}

uint16_t CanDCMD::adcCycleOnetime(){
	if(adcCycleTrigger){
		adcCycleTrigger = 0;
		return 1;
	}
	return 0;
}

uint16_t CanDCMD::errorTask(uint16_t errorValue){
	if(errorValue){

		if(motor[0]->motor->outEnable == 2 || motor[1]->motor->outEnable == 2){
			motor[0]->motor->buzzerStop();
			motor[1]->motor->buzzerStop();
		}

		motor[0]->motor->free();
		motor[1]->motor->free();		//�t���[(�o�͂��ׂăI�t)

		motor[0]->motor->outEnable = 0;
		motor[1]->motor->outEnable = 0;

		errorVoltage = vbattRead();
		errorCurrent[0] = currentValue[0];//currentRread(0);
		errorCurrent[1] = currentValue[1];//currentRread(1);
		driveErrorStat = driveStat;

		driveStat = DS_Error;

		motor[0]->ledOverRide(0);
		motor[1]->ledOverRide(0);					//duty���Ă邩�ǂ����͌��悤

		motor[0]->motorOverRide(1);
		motor[1]->motorOverRide(1);					//motor�N���X��outEnable��0�ɂ��Ă���̂Ťfree�ȊO�ĂׂȂ����Ǥ�ꉞ

		emg->emgRequest(errorValue);

		return driveError;
	}

	return 0;
}





float CanDCMD::vbattRead(){
	uint16_t channel = 2;
	if(adc->readStat[channel]){
		voltageValue = (float)adc->read(channel)*AdcToVoltageGain;
		vAve.stack(voltageValue);
		vMin.stack(voltageValue);
	}
	return voltageValue;
}

float CanDCMD::currentRread(uint16_t channel){
	if(channel > 1)return 0;
	if(adc->readStat[channel]){
		currentValue[channel] = (float)((float)adc->read(channel) * AdcToCurrentGain);

		cAve[channel].stack(currentValue[channel]);
		cMax[channel].stack(currentValue[channel]);
	}
	return currentValue[channel];
}
