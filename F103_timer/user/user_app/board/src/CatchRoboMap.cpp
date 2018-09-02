#include "CatchRoboMap.h"

void CatchRoboMap::setup(uint16_t _isR){
	isR = _isR;
	x = 0;
	y = 0;
	z = 0;
	notExist = 0;
}

void CatchRoboMap::callNormalWork(int stage,int col){
	x = InnerWall_NS1C1_X;
	switch(col){
	case 1:
		x += 0;
		break;

	case 2:
		x += 180;
		break;

	case 3:
		x += 180*2;
		break;

	case 4:
		x += 180*3;
		break;

	case 5:
		x += 180*4;
		break;

	case 6:
		x += 180*5;
		break;
	default:
		notExist = 1;
		return;
	}

	if(stage == 1){
		x += 0;
		y = InnerWall_NS1C1_Y - HandlingWorkPosition;
	}else if(stage == 2){
		x -= 90;
		y = InnerWall_NS2C1_Y + HandlingWorkPosition;
	}else{
		//ねーよ
		notExist = 1;
		return;
	}
	z = Field_Z;
}

void CatchRoboMap::callShareWorkR(int stage,int col){
	x = InnerWall_SS1C1_X;
	switch(col){
	case 1:
		x += 0;
		break;

	case 2:
		x += 80;
		break;

	case 3:
		x += 80*2;
		break;

	case 4:
		x += 80*3;
		break;

	case 5:
		x += 80*4;
		break;

	case 6:
		if(stage == 2)return;	//ねぇよ
		x += 80*5;
		break;
	default:
		notExist = 1;
		return;
	}

	if(stage == 1){
		y = InnerWall_SS1C1_Y;
	}else if(stage == 2){
		y = InnerWall_SS1C1_Y + 80;
		x += 40;
	}else if(stage == 3){
		y = InnerWall_SS1C1_Y + 80*2;
	}else{
		//ねーよ
		notExist = 1;
		return;
	}
	z = Field_Z;
}

void CatchRoboMap::callShareWorkL(int stage,int col){
	callShareWorkR(stage,col);
	x += 480 + 80;
}

void CatchRoboMap::callShootingBoxR(int row,int col){
	x = -InnerWall_BS1C1_X;
	switch(col){
	case 1:

		break;

	case 2:
		x += 80;
		break;

	case 3:
		x += 80*2;
		break;

	default:
		//ねーよ
		notExist = 1;
		return;
	}

	y = InnerWall_BS1C1_Y;
	switch(row){
	case 1:

		break;

	case 2:
		y += 80;
		break;

	case 3:
		y += 80*2;
		break;

	case 4:
		y += 80*3;
		break;

	default:
		//ねーよ
		notExist = 1;
		return;
	}
	z = ShootingBox_Z;
}

void CatchRoboMap::callShootingBoxL(int row,int col){
	callShootingBoxR(row,col);
	x += 1350 + 320;
}
