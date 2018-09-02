#ifndef CatchRobo_H_
#define CatchRobo_H_

#include "system.h"
#include "util.h"

#define InnerWall_NS1C1_X		240

#define InnerWall_NS1C1_Y		300		//ノーマル､ワークの先端
#define InnerWall_NS2C1_Y		210

#define InnerWall_SS1C1_Y		565		//シェア
#define InnerWall_SS1C1_X		165

#define HandlingWorkPosition 	125		//先端から保持高さまで


#define InnerWall_BS1C1_X		(240 + 30)	//BOX
#define InnerWall_BS1C1_Y		(80 + 50)


#define Field_Z					100
#define ShootingBox_Z			(450 + Field_Z)

class CatchRoboMap{
public:
	void setup(uint16_t isR);

	void callNormalWork(int stage,int col);
	void callShareWorkR(int stage,int col);
	void callShareWorkL(int stage,int col);
	void callShootingBoxR(int row,int col);
	void callShootingBoxL(int row,int col);		//行,列

	float x,y,z;

	uint16_t isR,notExist;
};


#endif
