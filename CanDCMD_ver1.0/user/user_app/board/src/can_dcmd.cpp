#include "can_dcmd.h"

void CanDCMD::canmdSetup(CanNodeMotorDriver &md0,CanNodeMotorDriver &md1,uint16_t buzzerBeepOrder){
	this->motor[0] = &md0;
	this->motor[1] = &md1;
	motor[0]->canMd_motor->free();
	motor[1]->canMd_motor->free();

	driveStat  = DS_NoPower;
	driveError = DE_None;

	onetimeTrigger = 0;

	overCurrentLimit[0] = 10;
	overCurrentLimit[1] = 10;

	adcCycleTrigger = 0;

	buzzerDelay = buzzerBeepOrder*100;
}


void CanDCMD::adcSetup(SI8900 &isoSet){
	this->adc = &isoSet;
}

void CanDCMD::emgSetup(CanNodeEmg  &emgSet){
	this->emg = &emgSet;
}

/**************************************Setup**********************************************/


void CanDCMD::cycle(){
	motor[0]->cycle();
	motor[1]->cycle();
	adc->cycle();

	cycleFunction();
}


void CanDCMD::cycleFunction(){

	if(adc->setupStat == 0 && driveStat != DS_NoPower){	//�p���[�Ȃ�(�p���[�n�ɓd�����Ȃ����adc.setupStat = 0�ɂȂ�)�̂�NoPower�ȊO�ɓ����Ă�����

		if(driveStat == DS_MotorBuzzer){
			motor[0]->canMd_motor->buzzerStop();
			motor[1]->canMd_motor->buzzerStop();
		}

		motor[0]->canMd_motor->free();
		motor[1]->canMd_motor->free();		//�t���[(�o�͂��ׂăI�t)

		motor[0]->canMd_motor->outEnable = 0;
		motor[1]->canMd_motor->outEnable = 0;

		motor[0]->ledOverRide(0);
		motor[1]->ledOverRide(0);					//CanNodeMotorDriver��led���삷��K�v�͂Ȃ�

		motor[0]->motorOverRide(1);
		motor[1]->motorOverRide(1);					//motor�N���X��outEnable��0�ɂ��Ă���̂Ťfree�ȊO�ĂׂȂ����Ǥ�ꉞ


		driveStat = DS_NoPower;

		return;
	}

	switch(driveStat){
	case DS_NoPower:
		if(adc->setupStat){
			motor[0]->canMd_motor->outEnable = 0;
			motor[1]->canMd_motor->outEnable = 0;	//���[�^�[�o�͖���(free�̂�)

			motor[0]->motorOverRide(1);
			motor[1]->motorOverRide(1);				//canNodeMotorDriver�̃��[�^�[�o�͊֐��Ăяo����������

			motor[0]->ledOverRide(1);
			motor[1]->ledOverRide(1);				//�N����������led�������ꎞ�I�ɒD��

			motor[0]->canMd_led->write(1);
			motor[1]->canMd_led->write(1);

			motor[0]->canMd_motor->free();
			motor[1]->canMd_motor->free();

			driveStat = DS_PowerIn;
			driveStatTimer = millis();
			onetimeTrigger = 1;

			vvMin = 1023;
			cvMax[0] = 0;
			cvMax[1] = 0;
		}

		break;

	case DS_PowerIn:

		if(millis() - adc->receiveTime[0] > 5)									driveError |= DE_ADCLost;		//�f�[�^���Ȃ�(�p���[�n�����Ă�H)
		if(millis() - driveStatTimer > SetupDerayTime)							driveError |= DE_UnderVoltage;	//�d�����Ȃ񂩕n��

		if(adc->readStat[ChannelVoltage] && adc->read(ChannelVoltage) <= voltageValue){	//�d���d���̏㏸���~�܂����玟�̏�Ԃ�
			driveStatTimer = millis();
			driveStat = DS_LowOn;
		}else{
			voltageValue = adc->read(ChannelVoltage);
		}

		if(errorTask(driveError))break;

		break;

	case DS_LowOn:

		if(adc->readStat[ChannelCurrentA] && currentRread(0) > SetupLimitCurrent)driveError |= DE_BreakFEToutAHigh;
		if(adc->readStat[ChannelCurrentB] && currentRread(1) > SetupLimitCurrent)driveError |= DE_BreakFEToutBHigh;
		if(adc->readStat[ChannelVoltage] && vbattRead()      < SetupLimitVoltage)driveError |= DE_UnderVoltage;
		if(millis() - adc->receiveTime[0] > 10)									 driveError |= DE_ADCLost;

		if(errorTask(driveError))break;																//�G���[�Ȃ炷��������

		if(millis() - driveStatTimer > 100){
			motor[0]->canMd_motor->pwm1->dutyF(0.5);
			motor[1]->canMd_motor->pwm1->dutyF(0.5);

			driveStatTimer = millis();
			driveStat = DS_HighOn;
		}
		break;

	case DS_HighOn:

		if(adc->readStat[ChannelCurrentA] && currentRread(0) > SetupLimitCurrent)driveError |= DE_BreakFEToutALow;
		if(adc->readStat[ChannelCurrentB] && currentRread(1) > SetupLimitCurrent)driveError |= DE_BreakFEToutBLow;
		if(adc->readStat[ChannelVoltage] && vbattRead() 	 < SetupLimitVoltage)driveError |= DE_UnderVoltage;
		if(millis() - adc->receiveTime[0] > 10)									 driveError |= DE_ADCLost;

		if(errorTask(driveError))break;																//�G���[�Ȃ炷��������

		if(millis() - driveStatTimer > 100){

			motor[0]->canMd_led->write(0);
			motor[1]->canMd_led->write(0);

			motor[0]->canMd_motor->free();
			motor[1]->canMd_motor->free();

			driveStatTimer = millis();
			driveStat = DS_MotorBuzzerDelay;
		}

		break;

	case DS_MotorBuzzerDelay:
		if(adc->readStat[ChannelVoltage] && vbattRead() 	 < SetupLimitVoltage)driveError |= DE_UnderVoltage;
		if(millis() - adc->receiveTime[0] > 10)									 driveError |= DE_ADCLost;

		if(errorTask(driveError))break;																//�G���[�Ȃ炷��������

		if(millis() - driveStatTimer > buzzerDelay){
			motor[0]->canMd_motor->outEnable = 1;
			motor[1]->canMd_motor->outEnable = 1;	//�u�U�[�g�����߂Ƀ��[�^�[�o�͗L��

			motor[0]->canMd_motor->buzzerStart(3000,0.05);
			motor[1]->canMd_motor->buzzerStart(3000,0.05);	//�u�U�[(500ms)

			driveStatTimer = millis();
			driveStat = DS_MotorBuzzer;
		}
		break;

	case DS_MotorBuzzer:

		if(adc->readStat[ChannelCurrentA] && currentRread(0) > BuzzerLimitCurrent)driveError |= DE_OCoutA;		//�u�U�[���[�h��������ĕی�͂���
		if(adc->readStat[ChannelCurrentB] && currentRread(1) > BuzzerLimitCurrent)driveError |= DE_OCoutB;
		if(adc->readStat[ChannelVoltage] && vbattRead() 	 < SetupLimitVoltage)driveError |= DE_UnderVoltage;
		if(millis() - adc->receiveTime[0] > 10)									 driveError |= DE_ADCLost;

		if(errorTask(driveError))break;																//�G���[�Ȃ炷��������

		if(millis() - driveStatTimer > 200){

			motor[0]->canMd_motor->buzzerStop();
			motor[1]->canMd_motor->buzzerStop();	//�u�U�[�����܂�

			motor[0]->motorOverRide(0);
			motor[1]->motorOverRide(0);				//canNodeMotorDriver�̃��[�^�[�o�͊֐��Ăяo�����L����

			motor[0]->canMd_motor->free();
			motor[1]->canMd_motor->free();			//�Ƃ肠����������

			motor[0]->ledOverRide(0);
			motor[1]->ledOverRide(0);				//led�����CanNodeMotorDriver�ɕԂ�

			driveStatTimer = millis();

			driveStat = DS_Drive;
		}
		break;

	case DS_Drive:
		if(adc->readStat[2]){
			adcCycleTrigger = 1;	//adc�S���ǂݏI�����(�H)�@������܂���
		}

		if(adc->readStat[ChannelCurrentA] && currentRread(0) > DriveLimitCurrent)driveError |= DE_OCoutA;
		if(adc->readStat[ChannelCurrentB] && currentRread(1) > DriveLimitCurrent)driveError |= DE_OCoutB;
		if(adc->readStat[ChannelVoltage]  && vbattRead() 	 < DriveLimitVoltage)driveError |= DE_UnderVoltage;
		if(millis() - adc->receiveTime[0] > 10)									 driveError |= DE_ADCLost;

		if(errorTask(driveError))break;

		break;
	case DS_Error:

		break;

	default:
		driveError = DE_Unknown;
		if(errorTask(driveError))break;

		break;
	}
}

/*********************************���펞�Ăяo���@���K�X�Ăяo��*****************************************/

void CanDCMD::overCurrentSet(uint16_t channel,float current_A){
	overCurrentLimit[channel] = current_A;
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

		if(driveStat == DS_MotorBuzzer){
			motor[0]->canMd_motor->buzzerStop();
			motor[1]->canMd_motor->buzzerStop();
		}

		motor[0]->canMd_motor->free();
		motor[1]->canMd_motor->free();		//�t���[(�o�͂��ׂăI�t)

		motor[0]->canMd_motor->outEnable = 0;
		motor[1]->canMd_motor->outEnable = 0;

		driveErrorStat = driveStat;
		driveStat = DS_Error;

		motor[0]->ledOverRide(0);
		motor[1]->ledOverRide(0);					//duty���Ă邩�ǂ����͌��悤

		motor[0]->motorOverRide(1);
		motor[1]->motorOverRide(1);					//motor�N���X��outEnable��0�ɂ��Ă���̂Ťfree�ȊO�ĂׂȂ����Ǥ�ꉞ

		//emg->emgRequest(errorValue);

		return driveError;
	}

	return 0;
}



float CanDCMD::vbattRead(){
	voltageValue = adc->read(2);
	if(voltageValue < vvMin)vvMin = voltageValue;
	return (float)(voltageValue * AdcToVoltageGain);
}


float CanDCMD::currentRread(uint16_t channel){
	if(channel > 1)return 0;
	currentValue[channel] = adc->read(channel);
	if(cvMax[channel] < currentValue[channel])cvMax[channel] = currentValue[channel];
	return (float)(currentValue[channel] * AdcToCurrentGain);
}
