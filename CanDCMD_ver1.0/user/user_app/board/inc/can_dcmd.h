#ifndef CAN_DCMD_H_
#define CAN_DCMD_H_

#include "system.h"
#include "app.h"

#define SetupDerayTime 100
#define AdcToCurrentGain (float)((3.3*50)/1023)
#define AdcToVoltageGain (float)((3.3*11)/1023)

#define SetupDelayTime 100

#define SetupLimitVoltage (float)(12.0)
#define SetupLimitCurrent (float)(10.0)


#define BuzzerLimitCurrent (float)(10.0)

#define DriveLimitVoltage (float)(12.0)
#define DriveLimitCurrent (float)(100.0)	//Å®overCurrentLimit[]

#define ChannelCurrentA 1
#define ChannelCurrentB 0
#define ChannelVoltage  2

enum DE_DriverError{
	DE_None 				= 0x000,
	DE_UnderVoltage 		= 0x001,
	DE_BreakFEToutAHigh		= 0x002,
	DE_BreakFEToutBHigh		= 0x004,
	DE_BreakFEToutALow 		= 0x008,
	DE_BreakFEToutBLow 		= 0x010,
	DE_OCoutA				= 0x020,
	DE_OCoutB				= 0x040,
	DE_ADCLost				= 0x080,
	DE_Unknown				= 0x100,
};

enum DS_DriveStat{
	DS_NoPower,
	DS_PowerIn,
	DS_LowOn,
	DS_HighOn,
	DS_MotorBuzzerDelay,
	DS_MotorBuzzer,
	DS_Drive,
	DS_Error,
};



class CanDCMD{
public:
	void canmdSetup(CanNodeMotorDriver &md0,CanNodeMotorDriver &md1,uint16_t buzzerBeepOrder = 0);
	void adcSetup(SI8900 &isoSet);
	void emgSetup(CanNodeEmg &emgSet);

	void cycle();
	uint16_t errorTask(uint16_t errorValue);

	void cycleFunction();

	uint16_t powerInOnetime();
	uint16_t adcCycleOnetime();		//ÇΩÇ‘ÇÒmainóp

	void overCurrentSet(uint16_t channel,float current_A);

	float vbattRead();
	float currentRread(uint16_t channel);

	uint16_t voltageValue,currentValue[2];
	uint16_t vvMin,cvMax[2],errorVoltageValue,errorCurrentValue[2];
	uint16_t driveStat;
	uint64_t driveStatTimer,driveLedTimer,buzzerDelay;

	uint16_t driveError,onetimeTrigger,adcCycleTrigger,driveErrorStat;

	float overCurrentLimit[2];

	CanNodeMotorDriver *motor[2];
	CanNodeEmg *emg;
	SI8900 *adc;
};



#endif
