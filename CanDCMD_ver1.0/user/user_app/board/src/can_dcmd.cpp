#include "can_dcmd.h"

void CanDCMD::canmdSetup(CanNodeMotorDriver &md0,CanNodeMotorDriver &md1){
	this->motor[0] = &md0;
	this->motor[1] = &md1;
	motor[0]->canMd_motor->free();
	motor[1]->canMd_motor->free();

	driveStat = 0;
	driveError = DE_None;

	onetimeTrigger = 0;
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
			if(vbattRead() < 20.0){
				driveError |= DE_UnderVoltage;
			}
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


	}

	if(adc->setupStat && powerIn == 0){
		motorDriverSetupSequence();				//パワー系に電源入った
		onetimeTrigger = 1;
	}

	powerIn = adc->setupStat;
}




uint16_t CanDCMD::motorDriverSetupSequence(){						//起動時保護処理
	uint64_t statTime = millis();

	while(1){														//adc一周読み込む
		adc->cycle();
		if(adc->readStat[0] || adc->readStat[1] || adc->readStat[2])break;
		if(millis() - statTime > SetupDerayTime)return 1;
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

		if(currentRread(0) > SetupLimitCurrent)driveError |= DE_BreakFEToutAHigh;
		if(currentRread(1) > SetupLimitCurrent)driveError |= DE_BreakFEToutBHigh;
		if(vbattRead() < SetupLimitVoltage)driveError |= DE_UnderVoltage;

		if(errorTask(driveError))return driveError;

		if(millis() - statTime > SetupDerayTime)return 1;
	}

	motor[0]->canMd_motor->pwm1->dutyF(0.5);
	motor[1]->canMd_motor->pwm1->dutyF(0.5);

	while(1){													//ハイサイド1のみON､ローサイド壊れてたらここで短絡
		adc->cycle();

		if(currentRread(0) > SetupLimitCurrent)driveError |= DE_BreakFEToutA1Low;
		if(currentRread(1) > SetupLimitCurrent)driveError |= DE_BreakFEToutB1Low;
		if(vbattRead() < SetupLimitVoltage)driveError |= DE_UnderVoltage;

		if(errorTask(driveError))return driveError;

		if(millis() - statTime > SetupDerayTime)return 1;
	}


	motor[0]->canMd_motor->pwm1->dutyF(0);
	motor[1]->canMd_motor->pwm1->dutyF(0);

	motor[0]->canMd_motor->pwm2->dutyF(0.5);
	motor[1]->canMd_motor->pwm2->dutyF(0.5);

	while(1){													//ハイサイド2のみON､ローサイド壊れてたらここで短絡
		adc->cycle();

		if(currentRread(0) > SetupLimitCurrent)driveError |= DE_BreakFEToutA2Low;
		if(currentRread(1) > SetupLimitCurrent)driveError |= DE_BreakFEToutB2Low;
		if(vbattRead() < SetupLimitVoltage)driveError |= DE_UnderVoltage;

		if(errorTask(driveError))return driveError;

		if(millis() - statTime > SetupDerayTime)return 1;
	}

	return 0;
}



uint16_t CanDCMD::powerInOnetime(){
	if(onetimeTrigger)return 0;
	return 1;
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
	if(adc->readStat[2])voltageValue = adc->read(2);
	return (float)(voltageValue * AdcToVoltageGain);
}


float CanDCMD::currentRread(uint16_t channel){
	if(channel > 1)return 0;
	if(adc->readStat[channel])currentValue[channel] = adc->read(channel);
	return (float)(currentValue[channel] * AdcToCurrentGain);
}


