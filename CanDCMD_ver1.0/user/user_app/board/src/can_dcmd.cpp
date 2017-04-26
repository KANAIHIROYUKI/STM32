#include "can_dcmd.h"

void CanDCMD::canmdSetup(CanNodeMotorDriver &md0,CanNodeMotorDriver &md1){
	this->motor[0] = &md0;
	this->motor[1] = &md1;
	motor[0]->canMd_motor->free();
	motor[1]->canMd_motor->free();

	driveStat = 0;
	driveError = DE_None;

	onetimeTrigger = 0;

	overCurrentLimit[0] = 10;
	overCurrentLimit[1] = 10;

	adcCycleTrigger = 0;
}


void CanDCMD::adcSetup(SI8900 &isoSet){
	this->adc = &isoSet;
	powerIn = 0;
}

/**************************************Setup**********************************************/


void CanDCMD::cycle(){
	if(driveError){								//エラー吐いてる時に回すモーターはありません
		motor[0]->canMd_motor->outEnable = 0;
		motor[1]->canMd_motor->outEnable = 0;
	}else{
		motor[0]->canMd_motor->outEnable = 1;
		motor[1]->canMd_motor->outEnable = 1;
	}


	motor[0]->cycle();
	motor[1]->cycle();
	adc->cycle();

	//まずはサイクルを回せ！

	if(powerIn){								//パワー系に電源入ってる(ADC読めてる
		if(adc->readStat[2]){
			if(vbattRead() < CycleLimitVoltage){
				driveError |= DE_UnderVoltage;
			}
			adcCycleTrigger = 1;
		}
		if(adc->readStat[0]){
			if(currentRread(0) > overCurrentLimit[0]){
				driveError |= DE_OCoutA;
			}
		}

		if(adc->readStat[1]){
			if(currentRread(1) > overCurrentLimit[1]){
				driveError |= DE_OCoutB;
			}
		}
	}else{

	}

	if(adc->setupStat && powerIn == 0){				//パワー系に電源入った
		driveStat = DS_PowerIn;
		motorDriverSetupSequence();
		onetimeTrigger = 1;
	}

	powerIn = adc->setupStat;
}


void CanDCMD::cycleFunction(){
	switch(driveStat){
	case DS_NoPower:

		break;
	case DS_PowerIn:

		break;
	case DS_LowOn:

		break;
	case DS_HighOn1:

		break;
	case DS_HighOn2:

		break;
	case DS_Drive:

		break;
	case DS_Error:

		break;
	default:

		break;
	}
}


uint16_t CanDCMD::motorDriverSetupSequence(){						//起動時保護処理







	uint64_t statTime = millis();




	for(int i=0;i<3;i++){
		adc->read(i);
		errorAdcValue[i] = 0;
	}

	while(1){														//adc一周読み込む
		adc->cycle();
		if(adc->readStat[0] && adc->readStat[1] && adc->readStat[2])break;
		if(millis() - statTime > SetupDerayTime)return 1;
		delay(1);
	}

	statTime = millis();

	motor[0]->canMd_motor->pwm1->duty(0);
	motor[0]->canMd_motor->pwm2->duty(0);
	motor[0]->canMd_motor->pwmEn->dutyF(1.0);

	motor[1]->canMd_motor->pwm1->duty(0);
	motor[1]->canMd_motor->pwm2->duty(0);
	motor[1]->canMd_motor->pwmEn->dutyF(1.0);

	while(1){													//ローサイドのみON､ハイサイド壊れてたらここで短絡
		adc->cycle();
		delay(1);

		if(adc->readStat[0] && currentRread(0) > SetupLimitCurrent)driveError |= DE_BreakFEToutAHigh;
		if(adc->readStat[1] && currentRread(1) > SetupLimitCurrent)driveError |= DE_BreakFEToutBHigh;
		if(adc->readStat[2] && vbattRead() < SetupLimitVoltage)driveError |= DE_UnderVoltage;

		if(errorTask(driveError)){

			return driveError;
		}

		if(millis() - statTime > SetupDerayTime)return 1;
	}

	motor[0]->canMd_motor->pwm1->dutyF(0.5);
	motor[1]->canMd_motor->pwm1->dutyF(0.5);

	while(1){													//ハイサイド1のみON､ローサイド壊れてたらここで短絡
		adc->cycle();
		delay(1);

		if(currentRread(0) > SetupLimitCurrent)driveError |= DE_BreakFEToutA1Low;
		if(currentRread(1) > SetupLimitCurrent)driveError |= DE_BreakFEToutB1Low;
		if(vbattRead() < SetupLimitVoltage)driveError |= DE_UnderVoltage;

		if(errorTask(driveError)){
			//errorAdcValue[0] = adc->read(0);
			//errorAdcValue[1] = adc->read(1);
			//errorAdcValue[2] = adc->read(2);
			return driveError;
		}
	}


	motor[0]->canMd_motor->pwm1->dutyF(0);
	motor[1]->canMd_motor->pwm1->dutyF(0);

	motor[0]->canMd_motor->pwm2->dutyF(0.5);
	motor[1]->canMd_motor->pwm2->dutyF(0.5);

	while(1){													//ハイサイド2のみON､ローサイド壊れてたらここで短絡
		adc->cycle();
		delay(1);

		if(currentRread(0) > SetupLimitCurrent)driveError |= DE_BreakFEToutA2Low;
		if(currentRread(1) > SetupLimitCurrent)driveError |= DE_BreakFEToutB2Low;
		if(vbattRead() < SetupLimitVoltage)driveError |= DE_UnderVoltage;

		if(errorTask(driveError)){
			//errorAdcValue[0] = adc->read(0);
			//errorAdcValue[1] = adc->read(1);
			//errorAdcValue[2] = adc->read(2);
			return driveError;
		}
	}

	return 0;
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
		motor[0]->canMd_motor->pwmEn->dutyF(0);
		motor[1]->canMd_motor->pwmEn->dutyF(0);

		motor[0]->canMd_motor->pwm1->duty(0);
		motor[0]->canMd_motor->pwm2->duty(0);

		motor[1]->canMd_motor->pwm1->duty(0);
		motor[1]->canMd_motor->pwm2->duty(0);

		//emg->emgRequest();




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


