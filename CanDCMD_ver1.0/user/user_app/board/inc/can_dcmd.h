#ifndef CAN_DCMD_H_
#define CAN_DCMD_H_

#include "system.h"
#include "app.h"

#define SetupDerayTime 100
#define AdcToCurrentGain (float)((3.3*50)/1023)
#define AdcToVoltageGain (float)((3.3*11)/1023)

#define SetupLimitCurrent (float)(1.0)
#define SetupLimitVoltage (float)(20.0)

enum EDriverError{
	DE_None 				= 0x000,
	DE_UnderVoltage 		= 0x001,
	DE_BreakFEToutAHigh		= 0x002,
	DE_BreakFEToutBHigh		= 0x004,
	DE_BreakFEToutA1Low 	= 0x008,
	DE_BreakFEToutB1Low		= 0x010,
	DE_BreakFEToutA2Low 	= 0x020,
	DE_BreakFEToutB2Low		= 0x040,
	DE_OCoutA				= 0x080,
	DE_OCoutB				= 0x100,
};


class CanDCMD{
public:
	void canmdSetup(CanNodeMotorDriver &md0,CanNodeMotorDriver &md1);
	void adcSetup(SI8900 &isoSet);
	void emgSetuo(CanNodeEmg &emgSet);

	void cycle();
	uint16_t errorTask(uint16_t errorValue);

	uint16_t powerInOnetime();

	float vbattRead();
	float currentRread(uint16_t channel);

	uint16_t motorDriverSetupSequence();

	uint16_t voltageValue,currentValue[2],powerIn;
	uint16_t driveStat,driveError,onetimeTrigger;

	float overCurrentLimit[2];

	CanNodeMotorDriver *motor[2];
	CanNodeEmg *emg;
	SI8900 *adc;
};



#endif
