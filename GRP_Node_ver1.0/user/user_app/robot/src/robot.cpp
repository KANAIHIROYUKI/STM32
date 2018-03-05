#include "robot.h"

void Robot::setup(){
	pointX = 0;
	pointY = 0;
	radian = 0;
}

void Robot::odometryEnc(){
	for(int i=0;i<3;i++){
		pointX += enc[i].read() * cos(radian + ENC_RAD[i]);
		pointY += enc[i].read() * sin(radian + ENC_RAD[i]);
	}
}

void Robot::odmetoryMouse(int x,int y){
	pointX += x*cos(radian) + y*sin(radian);
	pointY += x*sin(radian) + y*cos(radian);
}

void Robot::omni(float spd,float rad,float rps){
	float radi = -rad;

	for(int i=0;i<3;i++){

		while(radi < 0){
			radi += deg_to_radian(360);
		}
		while(radi > M_PI*2){
			radi -= deg_to_radian(360);
		}

		if(radi < deg_to_radian(60)){
			outDuty[i] = -1.0;
		}else if(radi < deg_to_radian(180)){
			outDuty[i] = (radi - M_PI*2/3)/ (M_PI/3);
		}else if(radi < deg_to_radian(240)){
			outDuty[i] = 1.0;
		}else if(radi < deg_to_radian(360)){
			outDuty[i] = -(radi - M_PI*5/3)/(M_PI/3);
		}

		outDuty[i] += rps;
		//motor[i].duty(outDuty[i]);
		//ƒ‚[ƒ^[‰ñ‚·Žž‚ÍƒRƒƒ“ƒgƒAƒEƒgŠO‚·

		radi += deg_to_radian(120);
	}

}

float Mecanum::move(float _x, float _y, float rev) {
	float deg = atan2(_y, _x) + M_PI / 4 - rad;

	while (deg < -M_PI * 3 / 4)deg += M_PI * 2;
	while (deg >  M_PI * 5 / 4)deg -= M_PI * 2;

	power = sqrt(_x * _x + _y * _y) / 100;
	if (power > 1)power = 1;

	if(deg < -M_PI/2){							//-PI*3/4 ` -PI/2
		out[0] = tan(M_PI / 2 - deg);
		out[1] = -1;
		div = 0;
	}
	else if(deg < -M_PI/4){						//-PI/2 ` -PI/4
		out[0] = -tan(deg + M_PI / 2);
		out[1] = -1;
		div = 1;
	}
	else if (deg < 0) {							//-PI/4@` 0
		out[0] = -1;
		out[1] = -tan(-deg);
		div = 2;
	}
	else if (deg < M_PI/4) {						//0@` PI/4
		out[0] = -1;
		out[1] = tan(deg);
		div = 3;
	}
	else if (deg < M_PI/2) {						//PI/4@` PI/2
		out[0] = -tan(M_PI /2 - deg);
		out[1] = 1;
		div = 4;
	}
	else if(deg < M_PI*3/4) {						//PI/2 ` PI*3/4
		out[0] = tan(deg - M_PI / 2);
		out[1] = 1;
		div = 5;
	}
	else if (deg < M_PI) {						//PI*3/4@` PI
		out[0] = 1;
		out[1] = tan(M_PI - deg);
		div = 6;
	}
	else{										//PI@` PI*5/4
		out[0] = 1;
		out[1] = -tan(deg - M_PI);
		div = 7;
	}


	out[2] = - out[0] * power - rev/10;
	out[3] = - out[1] * power - rev/10;
	out[0] = out[0] * power + rev/10;
	out[1] = out[1] * power + rev/10;

	float outMax = 0;
	for (int i = 0; i < 4; i++) {
		if (outMax < abs(out[i]))outMax = abs(out[i]);
	}
	if (outMax < 1)outMax = 1;
	for (int i = 0; i < 4; i++) {
		out[i] = out[i] / outMax * 10000;
	}

	return deg;
}
