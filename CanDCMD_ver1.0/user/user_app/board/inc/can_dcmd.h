#ifndef CAN_DCMD_H_
#define CAN_DCMD_H_

#include "system.h"
#include "app.h"

#define SetupDerayTime 100
#define AdcToCurrentGain (float)((3.3*50)/1023)
#define AdcToVoltageGain (float)((3.3*11)/1023)

#define SetupLimitCurrent (float)(1.0)
#define SetupLimitVoltage (float)(20.0)

/*
 * DRIVE ERRORS
 * 0:None
 * 1:Under voltage
 * 2:Break FET outA high
 * 3:Break FET outB high
 * 4:Break FET outA low
 * 5:Break FET outB low
 */



class CanDCMD{
public:
	void canmdSetup(CanNodeMotorDriver &md0,CanNodeMotorDriver &md1);
	void adcSetup(SI8900 &isoSet);
	void emgSetuo(CanNodeEmg &emgSet);

	void cycle();

	float vbattRead();
	float curentRread(uint16_t channel);

	uint16_t motorDriverSetupSequence();

	uint16_t voltageValue,currentValue[2],powerIn;
	uint16_t driveStat,driveError;

	CanNodeMotorDriver *motor[2];
	CanNodeEmg *emg;
	SI8900 *adc;
};



#endif
