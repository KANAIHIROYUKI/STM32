#include "motorController.h"

void MotorController::setup(DutyOut &motorSet,Enc &encSet,uint16_t cprSet){
	this->enc = &encSet;
	this->motor = &motorSet;
	cntPerRev = cprSet;

	controlCycleTime = millis();

	enc->reset();
}

void MotorController::cycle(){
	motor->cycle();

	if(millis() > controlCycleTime){
		controlCycleTime += MC_CONTROL_CYCLE;
		controlCycle();
	}
}


void MotorController::controlCycle(){
	cntPerTime = enc->read() - oldEncCnt;

	revPerSec = cntPerTime * (1000 / MC_CONTROL_CYCLE) / cntPerRev;
	currentPosition = enc->read() / cntPerRev;

	//‚½‚Ô‚ñÀ•Ww’è

	dif = targetPosition - currentPosition;
	difDif = revPerSec;
	difInt += dif;

	output = dif*kp + difDif*kd + difInt*kp;

	motor->duty(output);
}
