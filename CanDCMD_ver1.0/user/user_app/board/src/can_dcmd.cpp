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

	if(adc->setupStat == 0 && driveStat != DS_NoPower){	//パワーない(パワー系に電源がなければadc.setupStat = 0になる)のにNoPower以外に入っていた時

		if(driveStat == DS_MotorBuzzer){
			motor[0]->canMd_motor->buzzerStop();
			motor[1]->canMd_motor->buzzerStop();
		}

		motor[0]->canMd_motor->free();
		motor[1]->canMd_motor->free();		//フリー(出力すべてオフ)

		motor[0]->canMd_motor->outEnable = 0;
		motor[1]->canMd_motor->outEnable = 0;

		motor[0]->ledOverRide(0);
		motor[1]->ledOverRide(0);					//CanNodeMotorDriverがled操作する必要はない

		motor[0]->motorOverRide(1);
		motor[1]->motorOverRide(1);					//motorクラスのoutEnableを0にしているので､free以外呼べないけど､一応


		driveStat = DS_NoPower;

		return;
	}

	switch(driveStat){
	case DS_NoPower:
		if(adc->setupStat){
			motor[0]->canMd_motor->outEnable = 0;
			motor[1]->canMd_motor->outEnable = 0;	//モーター出力無効(freeのみ)

			motor[0]->motorOverRide(1);
			motor[1]->motorOverRide(1);				//canNodeMotorDriverのモーター出力関数呼び出しも無効化

			motor[0]->ledOverRide(1);
			motor[1]->ledOverRide(1);				//起動処理中はled処理を一時的に奪う

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

		if(millis() - adc->receiveTime[0] > 5)									driveError |= DE_ADCLost;		//データ来ない(パワー系落ちてる？)
		if(millis() - driveStatTimer > SetupDerayTime)							driveError |= DE_UnderVoltage;	//電源がなんか貧弱

		if(adc->readStat[ChannelVoltage] && adc->read(ChannelVoltage) <= voltageValue){	//電源電圧の上昇が止まったら次の状態へ
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

		if(errorTask(driveError))break;																//エラーならすぐ抜ける

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

		if(errorTask(driveError))break;																//エラーならすぐ抜ける

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

		if(errorTask(driveError))break;																//エラーならすぐ抜ける

		if(millis() - driveStatTimer > buzzerDelay){
			motor[0]->canMd_motor->outEnable = 1;
			motor[1]->canMd_motor->outEnable = 1;	//ブザー使うためにモーター出力有効

			motor[0]->canMd_motor->buzzerStart(3000,0.05);
			motor[1]->canMd_motor->buzzerStart(3000,0.05);	//ブザー(500ms)

			driveStatTimer = millis();
			driveStat = DS_MotorBuzzer;
		}
		break;

	case DS_MotorBuzzer:

		if(adc->readStat[ChannelCurrentA] && currentRread(0) > BuzzerLimitCurrent)driveError |= DE_OCoutA;		//ブザーモードだからって保護はつける
		if(adc->readStat[ChannelCurrentB] && currentRread(1) > BuzzerLimitCurrent)driveError |= DE_OCoutB;
		if(adc->readStat[ChannelVoltage] && vbattRead() 	 < SetupLimitVoltage)driveError |= DE_UnderVoltage;
		if(millis() - adc->receiveTime[0] > 10)									 driveError |= DE_ADCLost;

		if(errorTask(driveError))break;																//エラーならすぐ抜ける

		if(millis() - driveStatTimer > 200){

			motor[0]->canMd_motor->buzzerStop();
			motor[1]->canMd_motor->buzzerStop();	//ブザーおしまい

			motor[0]->motorOverRide(0);
			motor[1]->motorOverRide(0);				//canNodeMotorDriverのモーター出力関数呼び出しも有効化

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
			adcCycleTrigger = 1;	//adc全部読み終わった(？)　一周しました
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

/*********************************↑常時呼び出し　↓適宜呼び出し*****************************************/

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
		motor[1]->canMd_motor->free();		//フリー(出力すべてオフ)

		motor[0]->canMd_motor->outEnable = 0;
		motor[1]->canMd_motor->outEnable = 0;

		driveErrorStat = driveStat;
		driveStat = DS_Error;

		motor[0]->ledOverRide(0);
		motor[1]->ledOverRide(0);					//duty来てるかどうかは見よう

		motor[0]->motorOverRide(1);
		motor[1]->motorOverRide(1);					//motorクラスのoutEnableを0にしているので､free以外呼べないけど､一応

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
