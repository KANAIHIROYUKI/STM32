#include "CatchRoboMain.h"


void CatchRobo::setup(USART &serial,SBDBT &ps3,uint16_t _id){
	this->ps3 = &ps3;
	this->serial = &serial;
	id = _id;

	controlCycleTime = 10;	//制御周期

	pid[MotorX].setup(1.0,0.1,1.0);
	pid[MotorR].setup(1.0,0.1,1.0);
	pid[MotorS].setup(1.0,0.1,1.0);
	pid[MotorJ].setup(1.0,0.1,1.0);

	pid[ServoP].setup(1.0,0,0);
	pid[ServoZ].setup(1.0,0,0);

	motionType = Motion::wait;

	map.setup(id);
}

void CatchRobo::assignMD(CanMotorDriver &canMD,uint16_t address){
	this->md[address] = &canMD;
}

void CatchRobo::assignRX(CAN_RX &canRX,uint16_t address){
	this->rx[address] = &canRX;
}

void CatchRobo::assignAD(ADC &pot,uint16_t address){
	this->pot[address] = &pot;
}

void CatchRobo::assignENC(TIM &enc,uint16_t address){
	this->enc[address] = &enc;
}

void CatchRobo::assignValve(CanValve &_valve){
	this->valve = &_valve;
}

void CatchRobo::cycle(){
	if(ps3->update){									//データ受信時
		ps3->update = 0;
		outputEnable = 1;

		if(ps3->data[PS3_Start] == 1 && motionType == Motion::wait){
			motionType = Motion::home;
		}

		if(ps3->data[PS3_CIR] == 1 && motionType != Motion::get){
			motionType = Motion::get;
			endPosJ = 1000;
			endPosX = 500;
			endPosR = 60;
			endPosS = 500;
		}

		if(ps3->data[PS3_TRI] == 1 && motionType != Motion::jog){
			endPosR = readR();
			endPosX = readX();
			endPosS = readS();
		}

		if(ps3->data[PS3_R1] == 1 && ps3->oldData[PS3_R1] == 0){
			if(valveOut){
				valveOut = 0;
			}else{
				valveOut = 1;
			}
		}


	}else if(millis() - ps3->lastReceiveTime > 100){	//タイムアウト
		outputEnable = 0;


	}


	if(serial->available()){
		switch(serial->read()){
		case 'w':
			stageY++;
			break;
		case 'a':
			stageX++;
			break;
		case 's':
			stageY--;
			break;
		case 'd':
			stageX--;
			break;
		default:
			return;
		}
		if(stageY <= 3 && stageX >= 1 && stageX <= 6){
			map.callNormalWork(stageY,stageX);
			serial->printf("normal ");
		}

		if(stageY <= 4 && stageX < 1){
			map.callShootingBoxR(stageY,stageX + 4);
			serial->printf("shootR ");
		}

		if(stageY <= 4 && stageX > 6){
			map.callShootingBoxL(stageY,stageX - 6);
			serial->printf("shootL ");
		}

		if(stageY > 4 && stageX >= 1 && stageX <= 6){
			map.callShareWorkR(stageY - 4,stageX);
			serial->printf("shareR ");
		}

		if(stageY > 4 && stageX > 6 && stageX < 12){
			map.callShareWorkL(stageY - 4,stageX - 6);
			serial->printf("shareL ");
		}

		serial->printf("stageX = %d, stageY = %d, X = %4d, Y = %4d, Z = %4d  nE%d,\n\r",stageX,stageY,(int)map.x,(int)map.y,(int)map.z,map.notExist);
		map.notExist = 0;
	}

	if(millis() - cycleTime > controlCycleTime){			//制御周期
		cycleTime += controlCycleTime;
		controlCycle();
	}
}

void CatchRobo::controlCycle(){

	switch(motionType){
	case Motion::wait:

		break;
	case Motion::home:

		break;
	case Motion::get:

		break;
	case Motion::shoot:

		targetPosZ = endPosZ;
		if(readS() < 500){			//スライダがある程度縮んでからピッチ修正
			targetPosP = readR();
		}else{
			targetPosP = readR();
		}

		break;
	case Motion::attack:


		break;
	case Motion::jog:

		targetPosX += (ps3->data[PS3_HatLX] - 0x40) * 0.5;
		targetPosY += (ps3->data[PS3_HatRY] - 0x40) * 0.5;
		targetPosP = readR();
		break;
	}

	/***************************************************************/
	//ここではtergetX,Y,Zが指定されている
	float y_d = targetPosY + basePich_Offset_Y;
	float z_d = basePich_Offset_Z - targetPosZ;

	float virtualPitchR = atan2(y_d,z_d);
	float VirtualDistance = sqrt(y_d*y_d + z_d*z_d);

	targetPosS = sqrt(VirtualDistance*VirtualDistance - basePich_Offset_D*basePich_Offset_D);
	targetPosR = virtualPitchR; - atan2(basePich_Offset_D,VirtualDistance);
	targetPosR = radian_to_deg(targetPosR);
	/*
	pid[MotorX].input(targetPosX,readX());
	pid[MotorR].input(targetPosR,readR());
	pid[MotorS].input(targetPosS,readS());
	pid[MotorJ].input(targetPosJ,readJ());

	for(int i=0;i<4;i++){
		output[i] = pid[i].outputF();
	}
	output[MotorR] += sin(deg_to_radian(readR())) * readS() * MotorR_FF_S_Gain;
*/
	/*
	if(outputEnable){
		for(int i=0;i<4;i++){
			md[i]->duty(output[i]);
		}
		md[ServoZ]->position(targetPosZ);
		md[ServoP]->position(targetPosP);

		valve->write(0,valveOut);
	}else{
		for(int i=0;i<4;i++){
			md[i]->duty(0);
		}
	}*/
}

float CatchRobo::readR(){
	return pot[MotorR]->read() * 60 / 4096;
}

float CatchRobo::readX(){
	return enc[EncoderX]->read() * 800 / 4096;
}

float CatchRobo::readS(){
	return pot[MotorS]->read() * 1200 / 4096;
}

float CatchRobo::readJ(){
	return pot[MotorJ]->read() * 1500 / 4096;
}
