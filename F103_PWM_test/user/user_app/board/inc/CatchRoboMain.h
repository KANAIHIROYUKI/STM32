#ifndef CatchRoboMain_H_
#define CatchRoboMain_H_

#include "system.h"
#include "app.h"
#include "util.hpp"

class CatchRobo{
public:
	void setup(SBDBT &ps3);
	void assignMD(CanMotorDriver &canMD);
	void assignAD(ADC &pot);
	void assignENC(TIM &enc);

	SBDBT *ps3;
	uint32_t potCnt,mdCnt,encCnt;
	CanMotorDriver *md[4];
	ADC *pot[3];
	TIM *enc[2];
};






#endif
