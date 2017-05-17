#include "can_dcmd.h"

void CanDCMD::canmdSetup(CanNodeMotorDriver &md0,CanNodeMotorDriver &md1,uint16_t buzzerBeepOrder){
	this->motor[0] = &md0;
	this->motor[1] = &md1;
	motor[0]->canMd_motor->free();
	motor[1]->canMd_motor->free();

	driveStat  = DS_NoPower;
	driveError = DE_None;

	onetimeTrigger = 0;

	overCurrentLimit[0] = DriveLimitCurrent;
	overCurrentLimit[1] = DriveLimitCurrent;

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

	if(adc->readStat[2]){
		adcCycleTrigger = 1;	//adc�S���ǂݏI�����(�H)�@������܂���
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
		if(currentRread(ChannelCurrentA) > BuzzerLimitCurrent && adc->readStat[ChannelCurrentA])driveError |= DE_BreakFEToutAHigh;
		if(currentRread(ChannelCurrentB) > BuzzerLimitCurrent && adc->readStat[ChannelCurrentB])driveError |= DE_BreakFEToutBHigh;
		if(vbattRead() 	 < SetupLimitVoltage	&& adc->readStat[ChannelVoltage] )driveError |= DE_UnderVoltage;
		if(millis() - adc->receiveTime[0] > 10)									  driveError |= DE_ADCLost;

		if(errorTask(driveError))break;																//�G���[�Ȃ炷��������

		if(millis() - driveStatTimer > 100){
			motor[0]->canMd_motor->pwm1->dutyF(0.5);
			motor[1]->canMd_motor->pwm1->dutyF(0.5);

			driveStatTimer = millis();
			driveStat = DS_HighOn;
		}
		break;

	case DS_HighOn:
		if(currentRread(ChannelCurrentA) > BuzzerLimitCurrent && adc->readStat[ChannelCurrentA])driveError |= DE_BreakFEToutALow;
		if(currentRread(ChannelCurrentB) > BuzzerLimitCurrent && adc->readStat[ChannelCurrentB])driveError |= DE_BreakFEToutBLow;
		if(vbattRead() 	 < SetupLimitVoltage	&& adc->readStat[ChannelVoltage] )driveError |= DE_UnderVoltage;
		if(millis() - adc->receiveTime[0] > 10)									  driveError |= DE_ADCLost;

		if(errorTask(driveError))break;																//�G���[�Ȃ炷��������

		if(millis() - driveStatTimer > 100){

			motor[0]->canMd_led->write(0);
			motor[1]->canMd_led->write(0);

			motor[0]->canMd_motor->outEnable = 1;
			motor[1]->canMd_motor->outEnable = 1;	//�u�U�[�g�����߂Ƀ��[�^�[�o�͗L��

			motor[0]->canMd_motor->buzzerStart(BuzzerFrq,0.1);
			motor[1]->canMd_motor->buzzerStart(BuzzerFrq,0.1);	//�u�U�[(50ms)

			buzzerTest[0] = 0;
			buzzerTest[1] = 0;

			driveStatTimer = millis();
			driveStat = DS_MotorBuzzerCurrentTest;
		}

		break;

	case DS_MotorBuzzerCurrentTest:
		if(currentRread(ChannelCurrentA) > BuzzerLimitCurrent && adc->readStat[ChannelCurrentA])driveError |= DE_OCoutA;		//�u�U�[���[�h��������ĕی�͂���
		if(currentRread(ChannelCurrentB) > BuzzerLimitCurrent && adc->readStat[ChannelCurrentB])driveError |= DE_OCoutB;
		if(vbattRead() 	 < SetupLimitVoltage	&& adc->readStat[ChannelVoltage])driveError |= DE_UnderVoltage;
		if(millis() - adc->receiveTime[0] > 10)									 driveError |= DE_ADCLost;

		if(currentRread(ChannelCurrentA) > buzzerTest[ChannelCurrentA])buzzerTest[ChannelCurrentA] = currentRread(ChannelCurrentA);
		if(currentRread(ChannelCurrentB) > buzzerTest[ChannelCurrentB])buzzerTest[ChannelCurrentB] = currentRread(ChannelCurrentB);

		if(millis() - driveStatTimer > 20){
			motor[0]->canMd_motor->buzzerStart(BuzzerFrq,0);
			motor[1]->canMd_motor->buzzerStart(BuzzerFrq,0);		//�u�U�[OFF

			buzzerTest[ChannelCurrentA] = 0.1*BuzzerTargetCurrent/buzzerTest[ChannelCurrentA];
			buzzerTest[ChannelCurrentB] = 0.1*BuzzerTargetCurrent/buzzerTest[ChannelCurrentB];

			if(buzzerTest[ChannelCurrentA] >= 0.5)buzzerTest[ChannelCurrentA] = 0.5;
			if(buzzerTest[ChannelCurrentB] >= 0.5)buzzerTest[ChannelCurrentB] = 0.5;

			driveStatTimer = millis();
			driveStat = DS_MotorBuzzerDelay;
		}

		break;
	case DS_MotorBuzzerDelay:
		if(adc->readStat[ChannelVoltage] && vbattRead() 	 < SetupLimitVoltage)driveError |= DE_UnderVoltage;
		if(millis() - adc->receiveTime[0] > 10)									 driveError |= DE_ADCLost;

		if(errorTask(driveError))break;																//�G���[�Ȃ炷��������

		if(millis() - driveStatTimer > buzzerDelay){
			motor[0]->canMd_motor->buzzerStart(BuzzerFrq,buzzerTest[ChannelCurrentA]);
			motor[1]->canMd_motor->buzzerStart(BuzzerFrq,buzzerTest[ChannelCurrentB]);	//�u�U�[(500ms)

			buzzerTest[ChannelCurrentA] = 0;
			buzzerTest[ChannelCurrentB] = 0;

			driveStatTimer = millis();
			driveStat = DS_MotorBuzzer;
		}
		break;

	case DS_MotorBuzzer:
		if(currentRread(ChannelCurrentA) > BuzzerLimitCurrent && adc->readStat[ChannelCurrentA])driveError |= DE_OCoutA;
		if(currentRread(ChannelCurrentB) > BuzzerLimitCurrent && adc->readStat[ChannelCurrentB])driveError |= DE_OCoutB;
		if(vbattRead() 	 < SetupLimitVoltage	&& adc->readStat[ChannelVoltage] )driveError |= DE_UnderVoltage;
		if(millis() - adc->receiveTime[0] > 10)									  driveError |= DE_ADCLost;

		if(errorTask(driveError))break;																//�G���[�Ȃ炷��������

		if(currentRread(ChannelCurrentA) > buzzerTest[ChannelCurrentA])buzzerTest[ChannelCurrentA] = currentRread(ChannelCurrentA);
		if(currentRread(ChannelCurrentB) > buzzerTest[ChannelCurrentB])buzzerTest[ChannelCurrentB] = currentRread(ChannelCurrentB);

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
		if(currentRread(ChannelCurrentA) > overCurrentLimit[ChannelCurrentA]/* && adc->readStat[ChannelCurrentA]*/)driveError |= DE_OCoutA;
		if(currentRread(ChannelCurrentB) > overCurrentLimit[ChannelCurrentB]/* && adc->readStat[ChannelCurrentB]*/)driveError |= DE_OCoutB;
		if(vbattRead() 	 < SetupLimitVoltage	&& adc->readStat[ChannelVoltage] )driveError |= DE_UnderVoltage;
		if(millis() - adc->receiveTime[0] > 10)									  driveError |= DE_ADCLost;

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





uint16_t CanDCMD::aveRead(uint16_t channel){
	uint16_t aveReturn = aveVal[channel]/aveCnt[channel];
	aveCnt[channel] = 0;
	aveVal[channel] = 0;
	return aveReturn;
}

uint16_t CanDCMD::maxRead(uint16_t channel){
	uint16_t maxReturn = maxVal[channel];
	maxVal[channel] = 0;
	return maxReturn;
}

uint16_t CanDCMD::minRead(uint16_t channel){
	uint16_t minReturn = minVal[channel];
	minVal[channel] = 1023;
	return minReturn;
}

float CanDCMD::vMaxRead(){
	return maxRead(ChannelVoltage) * AdcToVoltageGain;
}

float CanDCMD::vAveRead(){
	return aveRead(ChannelVoltage) * AdcToVoltageGain;
}

float CanDCMD::vMinRead(){
	return minRead(ChannelVoltage) * AdcToVoltageGain;
}

float CanDCMD::cMaxRead(uint16_t channel){
	return maxRead(channel) * AdcToCurrentGain * hardwareOption;
}

float CanDCMD::cAveRead(uint16_t channel){
	return aveRead(channel) * AdcToCurrentGain * hardwareOption;
}

float CanDCMD::vbattRead(){
	uint16_t channel = 2;
	if(adc->readStat[channel]){
		voltageValue = adc->read(channel);
		aveVal[channel] += voltageValue;
		aveCnt[channel]++;

		if(minVal[channel] > voltageValue)minVal[channel] = voltageValue;
		if(maxVal[channel] < voltageValue)maxVal[channel] = voltageValue;
	}
	return (float)((float)voltageValue * AdcToVoltageGain);
}

float CanDCMD::currentRread(uint16_t channel){
	if(channel > 1)return 0;
	if(adc->readStat[channel]){
		currentValue[channel] = adc->read(channel);
		aveVal[channel] += currentValue[channel];
		aveCnt[channel]++;

		if(minVal[channel] > currentValue[channel])minVal[channel] = currentValue[channel];
		if(maxVal[channel] < currentValue[channel])maxVal[channel] = currentValue[channel];
	}
	return (float)((float)currentValue[channel] * AdcToCurrentGain * (float)hardwareOption);
}
