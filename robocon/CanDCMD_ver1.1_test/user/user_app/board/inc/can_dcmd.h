#ifndef CAN_DCMD_H_
#define CAN_DCMD_H_

#include "system.h"
#include "app.h"
#include "math.h"
#include "util.hpp"

#define ChannelCurrentA 1
#define ChannelCurrentB 0
#define ChannelVoltage  2

#define SetupDerayTime 100
#define AdcToCurrentGain (float)((3.3*50)/1023)
#define AdcToVoltageGain (float)((3.3*11)/1023)

#define SetupDelayTime 100

#define SetupLimitVoltage (float)(12.0)
#define SetupOverVoltage  (float)(26.0)
#define SetupLimitCurrent (float)(15.0)

#define BuzzerLimitCurrent (float)(18.0)
#define BuzzerFrq			4000
#define BuzzerTargetCurrent 5

#define DriveLimitVoltage (float)(12.0)
#define DriveLimitCurrent (float)(100.0)	//→overCurrentLimit[]


#define DriveIntervalTime 50

enum DE_DriverError{
	DE_None 				= 0x0000,
	DE_UnderVoltage 		= 0x0000,		//MD側で読むとデメリットのほうが多そうなので切ってる
	DE_OverVoltage			= 0x0002,
	DE_BreakFEToutAHigh		= 0x0004,
	DE_BreakFEToutBHigh		= 0x0008,
	DE_BreakFEToutALow 		= 0x0010,
	DE_BreakFEToutBLow 		= 0x0020,
	DE_OCoutA				= 0x0040,
	DE_OCoutB				= 0x0080,
	DE_AveOCoutA			= 0x0100,
	DE_AveOCoutB			= 0x0200,
	DE_ADCLost				= 0x0000,
	DE_UnderVoltageAve		= 0x0000,		//同様
	DE_Unknown				= 0x1000,
};

enum DS_DriveStat{
	DS_NoPower,
	DS_PowerIn,
	DS_LowOn,
	DS_HighOn,
	DS_MotorBuzzerCurrentTest,
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
	void optionSetup(GPIO &optionSet);

	void cycle();
	uint16_t errorTask(uint16_t errorValue);

	void cycleFunction();

	uint16_t powerInOnetime();
	uint16_t adcCycleOnetime();		//たぶんmain用

	void overCurrentPeakSet(uint16_t channel,float current_A);
	void overCurrentAveSet(uint16_t channel,float current_A);

	float vbattRead();
	float currentRread(uint16_t channel);

	uint16_t hardwareOption;

	uint16_t driveStat;
	uint64_t driveStatTimer,buzzerDelay;
	uint16_t driveError,onetimeTrigger,adcCycleTrigger;

	uint16_t driveErrorStat;

	float buzzerTest[2],voltageValue,currentValue[2],setupvoltageValue;
	float errorVoltage,errorCurrent[2];

	float overCurrentLimit[2],overCurrentLimitAve[2];
	float underVoltageLimit,underVoltageLimitAve;

	float printcMax[2],printcAve[2],printvAve,printvMin;

	CanNodeMotorDriver *motor[2];
	CanNodeEmg *emg;
	SI8900 *adc;
	GPIO *option;

	Average<float> vAve,cAve[2];
	Maximum<float> cMax[2];
	Minimum<float> vMin;
};



#endif
