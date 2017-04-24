#include "can_dcmd.h"

void CanDCMD::canmdSetup(CanNodeMotorDriver &md0,CanNodeMotorDriver &md1){
	this->motor[0] = &md0;
	this->motor[1] = &md1;
	motor[0]->canMd_motor->free();
	motor[1]->canMd_motor->free();

	driveStat = 0;
	driveError = 0;
}

void CanDCMD::adcSetup(SI8900 &isoSet){
	this->adc = &isoSet;
	powerIn = 0;
}


void CanDCMD::cycle(){
	motor[0]->cycle();
	motor[1]->cycle();
	adc->cycle();

	//まずはサイクルを回せ！

	if(powerIn){
		if(vbattRead() < 20){
			driveError = 1;
		}
	}

	if(adc->setupStat && powerIn == 0){
		motorDriverSetupSequence();				//パワー系に電源入った
	}
	powerIn = adc->setupStat;
}



float CanDCMD::vbattRead(){
	if(adc->readStat[2])voltageValue = adc->read(2);
	return (float)(voltageValue * AdcToVoltageGain);
}

float CanDCMD::curentRread(uint16_t channel){
	if(channel < 1)return 0;
	if(adc->readStat[channel])voltageValue = adc->read(channel);
	return (float)(currentValue[channel] * AdcToCurrentGain);
}

uint16_t CanDCMD::motorDriverSetupSequence(){
	uint16_t phaseStat = 0;
	uint64_t statTime = millis() + SetupDerayTime;


	while(1){
		adc->cycle();
		if(adc->readStat[0] || adc->readStat[1] || adc->readStat[2]){
			break;
		}

		if(millis() - statTime > 100)return 1;
	}

	while(1){
		adc->cycle();

		if(millis() > statTime){
			statTime += SetupDerayTime;
			phaseStat++;
		}

		if(curentRread(0) > SetupLimitCurrent){

		}

		if(curentRread(1) > SetupLimitCurrent){

		}

		if(vbattRead() < SetupLimitVoltage){
			driveError = 1;
		}

		if(driveError){
			motor[0]->canMd_motor->pwmEn->dutyF(0);
			motor[1]->canMd_motor->pwmEn->dutyF(0);

			motor[0]->canMd_motor->pwm1->duty(0);
			motor[0]->canMd_motor->pwm2->duty(0);

			motor[1]->canMd_motor->pwm1->duty(0);
			motor[1]->canMd_motor->pwm2->duty(0);

			//emg->emgRequest();

			return driveError;
		}

		switch(phaseStat){
		case 0:
			motor[0]->canMd_motor->pwm1->duty(0);
			motor[0]->canMd_motor->pwm2->duty(0);
			motor[0]->canMd_motor->pwmEn->dutyF(1.0);

			motor[1]->canMd_motor->pwm1->duty(0);
			motor[1]->canMd_motor->pwm2->duty(0);
			motor[1]->canMd_motor->pwmEn->dutyF(1.0);
			break;

		case 1:
			motor[0]->canMd_motor->pwm1->duty(0.5);
			motor[0]->canMd_motor->pwm2->duty(0.5);
			motor[0]->canMd_motor->pwmEn->dutyF(1.0);

			motor[1]->canMd_motor->pwm1->duty(0.5);
			motor[1]->canMd_motor->pwm2->duty(0.5);
			motor[1]->canMd_motor->pwmEn->dutyF(1.0);
			break;

		default:
			return 0;
			break;
		}
	}
	return 0;
}
