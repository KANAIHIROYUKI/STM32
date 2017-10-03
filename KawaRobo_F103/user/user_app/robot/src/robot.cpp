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
		//モーター回す時はコメントアウト外す

		radi += deg_to_radian(120);
	}

}
