#ifndef CatchRoboMain_H_
#define CatchRoboMain_H_

#include "system.h"
#include "app.h"
#include "util.h"
#include "util.hpp"
#include "math.h"
#include "CatchRoboMap.h"

#define MotorX 0
#define MotorR 1
#define MotorS 2
#define MotorJ 3
#define ServoZ 4
#define ServoP 5

#define PosX 0
#define PosY 1
#define PosZ 2
#define PosA 3

#define EncoderX 0
#define EncoderR 1

#define	PotentioS 0
#define PotentioJ 1

#define RobotR_FirstWorkStage	2
#define RobotR_FirstWorkColumn	3

#define RobotL_FirstWorkStage	2
#define RobotL_FirstWorkColumn	5

namespace Motion{
	enum MotionType{
		wait = 0,
		origin,
		home,
		workField,
		shootingBox,
	};
}

namespace Position{

}


#define MotorR_FF_S_Gain 0.2
#define MotorS_FF_S_Gain 0.15



#define basePich_Offset_D	40
#define basePich_Offset_Y	200
#define basePich_Offset_Z	(450 + 500)		//フィールド底面←→ピッチ付け根

#define ENC_CNT (2000*4)
#define DistanceToCntX (8000/50/M_PI)

class CatchRobo{
public:
	void setup(USART &serial,SBDBT &ps3,uint16_t id);
	void assignMD(CanMotorDriver &canMD,uint16_t address);
	void assignRX(CAN_RX &canRX,uint16_t address);
	void assignAD(ADC &pot,uint16_t address);
	void assignENC(TIM &enc,uint16_t address);
	void assignLimit(Switch &sw,uint16_t address);
	void assignValve(CanValve &valve);

	void cycle();
	void controlCycle();
	void armControl();
	void motorControl();

	float readR();
	float readX();
	float readS();
	float readJ();

	void offsetJog();
	void offsetClear();

	uint64_t cycleTime,controlCycleTime,printCycleTime;

	USART *serial;
	SBDBT *ps3;
	uint16_t id;
	CanMotorDriver *md[6];
	CAN_RX *rx[2];
	ADC *pot[3];
	TIM *enc[2];
	Switch *limit[2];
	CanValve *valve;

	CatchRoboMap map;

	PID pid[6];
	float output[4];
	float targetPosX,targetPosR,targetPosS,targetPosJ,targetPosY;
	float targetPosP,targetPosZ,targetPosA;

	float offsetPos[6];

	uint16_t origin,originR,handInternal;
	uint16_t motionType;
	int16_t stageX,stageY,boxX,boxY;
	uint16_t handState,getState;
	float rFF,servoDuty[2],sFF;

	uint16_t swStat[20];
	uint8_t vData[8];
	uint16_t escape;
};






#endif
