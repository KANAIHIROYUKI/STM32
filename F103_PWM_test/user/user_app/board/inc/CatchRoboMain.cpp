#include "CatchRoboMain.h"


void CatchRobo::setup(SBDBT &ps3){
	this->ps3 = &ps3;

	potCnt = 0;
	encCnt = 0;
	mdCnt = 0;
}

void CatchRobo::assignMD(CanMotorDriver &canMD){
	this->md[mdCnt] = &canMD;
	mdCnt++;
}

void CatchRobo::assignAD(ADC &pot){
	this->pot[potCnt] = &pot;
	potCnt++;
}

void CatchRobo::assignENC(TIM &enc){
	this->enc[encCnt] = &enc;
	encCnt++;
}
