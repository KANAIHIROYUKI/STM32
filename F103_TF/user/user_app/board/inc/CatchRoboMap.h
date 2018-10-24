#ifndef CatchRobo_H_
#define CatchRobo_H_

#include "system.h"
#include "util.h"

#define InnerWall_NS1C1_X		150	//240�ɂȂ��Ă�����H�H�H

#define InnerWall_NS1C1_Y		300		//�m�[�}������[�N�̐�[
#define InnerWall_NS2C1_Y		210

#define InnerWall_SS1C1_Y		565		//�V�F�A
#define InnerWall_SS1C1_X		165

#define HandlingWorkPosition 	150		//��[����ێ������܂�


#define InnerWall_BS1C1_X		(80 + 30)	//BOX
#define InnerWall_BS1C1_Y		(80 + 50)


#define Field_Z					180
#define ShootingBox_Z			(280 + Field_Z)

#define Field_Y					-30

class CatchRoboMap{
public:
	void setup(uint16_t isR);

	void callNormalWork(int stage,int col);
	void callShareWorkR(int stage,int col);
	void callShareWorkL(int stage,int col);
	void callShootingBoxR(int row,int col);
	void callShootingBoxL(int row,int col);		//�s,��

	void callWork(int row,int col);
	void callShootingBox(int row,int col);

	float x,y,z;

	uint16_t isR,notExist;
};


#endif
