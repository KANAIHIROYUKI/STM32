#ifndef _ROBOT_H_
#define _ROBOT_H_

#include "system.h"
#include "enc.h"
#include "math.h"
#include "base.h"
#include "util.h"

#define ENC0_RAD ( 60/180)*M_PI
#define ENC1_RAD (180/180)*M_PI
#define ENC2_RAD (300/180)*M_PI

#define MOTOR0_RAD ( 60/180)*M_PI
#define MOTOR1_RAD (180/180)*M_PI
#define MOTOR2_RAD (300/180)*M_PI

const float ENC_RAD[3] = {ENC0_RAD,ENC1_RAD,ENC2_RAD};
const float MOTOR_RAD[3] = {MOTOR0_RAD,MOTOR1_RAD,MOTOR2_RAD};

class Robot{
public:
	float radian,pointX,pointY;
	float encRad[3],outDuty[3];

	void setup();
	void odometryEnc();
	void odmetoryMouse(int x,int y);
	void omni(float spd,float rad,float rps);

	Enc enc[3];
	DutyOut motor[3];
};

class Mecanum {
public:
	float out[4],power;

	float move(float x, float y, float rad);

	float x, y, rad,outRad,posX[4],posY[4],velX,velY,velR;
	int div,deg2;
};


#endif
