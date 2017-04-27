#include "can_dcmd.h"

void CanDCMD::canmdSetup(CanNodeMotorDriver &md0,CanNodeMotorDriver &md1){
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
}


void CanDCMD::adcSetup(SI8900 &isoSet){
	this->adc = &isoSet;
}

/**************************************Setup**********************************************/


void CanDCMD::cycle(){
	motor[0]->cycle();
	motor[1]->cycle();
	adc->cycle();

	cycleFunction();
}


void CanDCMD::cycleFunction(){
	switch(driveStat){
	case DS_NoPower:
		if(adc->setupStat){
			driveStat = DS_PowerIn;
			onetimeTrigger = 1;
		}

		break;

	case DS_PowerIn:
		motor[0]->canMd_motor->outEnable = 0;
		motor[1]->canMd_motor->outEnable = 0;	//モーター出力無効

		motor[0]->ledOverRide(1);
		motor[1]->ledOverRide(1);				//起動処理中はled処理を一時的に奪う

		driveStatTimer = millis();
		driveStat = DS_LowOn;

		motor[0]->canMd_motor->pwm1->duty(0);
		motor[0]->canMd_motor->pwm2->duty(0);
		motor[0]->canMd_motor->pwmEn->dutyF(1.0);

		motor[1]->canMd_motor->pwm1->duty(0);
		motor[1]->canMd_motor->pwm2->duty(0);
		motor[1]->canMd_motor->pwmEn->dutyF(1.0);

		break;

	case DS_LowOn:

		if(adc->readStat[ChannelCurrent0] && currentRread(0) > SetupLimitCurrent)driveError |= DE_BreakFEToutAHigh;
		if(adc->readStat[ChannelCurrent1] && currentRread(1) > SetupLimitCurrent)driveError |= DE_BreakFEToutBHigh;
		if(adc->readStat[ChannelVoltage] && vbattRead()      < SetupLimitVoltage)driveError |= DE_UnderVoltage;

		if(errorTask(driveError))break;

		if(millis() - driveStatTimer > 100){
			motor[0]->canMd_motor->pwm1->dutyF(0.5);
			motor[1]->canMd_motor->pwm1->dutyF(0.5);

			driveStatTimer = millis();
			driveStat = DS_HighOn1;
		}
		break;

	case DS_HighOn1:
		if(adc->readStat[ChannelCurrent0] && currentRread(0) > SetupLimitCurrent)driveError |= DE_BreakFEToutA1Low;
		if(adc->readStat[ChannelCurrent1] && currentRread(1) > SetupLimitCurrent)driveError |= DE_BreakFEToutB1Low;
		if(adc->readStat[ChannelVoltage] && vbattRead() 	 < SetupLimitVoltage)driveError |= DE_UnderVoltage;

		if(errorTask(driveError))break;

		if(millis() - driveStatTimer > 100){
			motor[0]->canMd_motor->pwm2->dutyF(0);
			motor[1]->canMd_motor->pwm2->dutyF(0);

			motor[0]->canMd_motor->pwm2->dutyF(0.5);
			motor[1]->canMd_motor->pwm2->dutyF(0.5);

			driveStatTimer = millis();
			driveStat = DS_HighOn2;
		}
		break;

	case DS_HighOn2:

		if(adc->readStat[ChannelCurrent0] && currentRread(0) > SetupLimitCurrent)driveError |= DE_BreakFEToutA2Low;
		if(adc->readStat[ChannelCurrent1] && currentRread(1) > SetupLimitCurrent)driveError |= DE_BreakFEToutB2Low;
		if(adc->readStat[ChannelVoltage] && vbattRead() 	 < SetupLimitVoltage)driveError |= DE_UnderVoltage;

		if(errorTask(driveError))break;

		if(millis() - driveStatTimer > 100){
			motor[0]->canMd_motor->outEnable = 1;
			motor[1]->canMd_motor->outEnable = 1;	//モーター出力有効

			motor[0]->canMd_motor->free();
			motor[1]->canMd_motor->free();			//とりあえず初期化

			motor[0]->ledOverRide(0);
			motor[1]->ledOverRide(0);				//led操作をCanNodeMotorDriverに返す

			driveStatTimer = millis();
			driveStat = DS_Drive;
		}

		break;

	case DS_Drive:
		if(adc->readStat[2]){
			adcCycleTrigger = 1;
		}

		if(adc->readStat[ChannelCurrent0] && currentRread(0) > SetupLimitCurrent)driveError |= DE_OCoutA;
		if(adc->readStat[ChannelCurrent1] && currentRread(1) > SetupLimitCurrent)driveError |= DE_OCoutB;
		if(adc->readStat[ChannelVoltage]  && vbattRead() 	 < SetupLimitVoltage)driveError |= DE_UnderVoltage;

		if(errorTask(driveError))break;

		break;
	case DS_Error:

		break;
	default:

		break;
	}
}

/*********************************↑常時呼び出し　↓適宜呼び出し*****************************************/

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
		motor[0]->canMd_motor->pwmEn->dutyF(0);
		motor[1]->canMd_motor->pwmEn->dutyF(0);

		motor[0]->canMd_motor->pwm1->duty(0);
		motor[0]->canMd_motor->pwm2->duty(0);

		motor[1]->canMd_motor->pwm1->duty(0);
		motor[1]->canMd_motor->pwm2->duty(0);

		driveStat = DS_Error;
		//emg->emgRequest();

		motor[0]->ledOverRide(1);
		motor[1]->ledOverRide(1);					//CanNodeMotorDriveがled操作してる場合じゃねぇ

		motor[0]->canMd_motor->outEnable = 0;
		motor[1]->canMd_motor->outEnable = 0;		//モーター出力有効

		return driveError;
	}

	return 0;
}



float CanDCMD::vbattRead(){
	voltageValue = adc->read(2);
	return (float)(voltageValue * AdcToVoltageGain);
}


float CanDCMD::currentRread(uint16_t channel){
	if(channel > 1)return 0;
	currentValue[channel] = adc->read(channel);
	return (float)(currentValue[channel] * AdcToCurrentGain);
}
