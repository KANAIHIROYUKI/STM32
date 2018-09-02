#include "CatchRoboMain.h"


void CatchRobo::setup(USART &serial,SBDBT &ps3,uint16_t _id){
	this->ps3 = &ps3;
	this->serial = &serial;
	id = _id;

	controlCycleTime = 10;	//制御周期

	pid[MotorX].setup(0.025,	0,	0.2);
	pid[MotorR].setup(4,		0,	10);
	pid[MotorS].setup(0.01,		0,	0.01);
	pid[MotorJ].setup(0.01,		0,	0.01);

	pid[ServoP].setup(1.0,0,0);
	pid[ServoZ].setup(1.0,0,0);

	for(int i=0;i<6;i++){
		pid[i].outLimit(-1.0,1.0);
	}

	motionType = Motion::wait;
	handState = 0;

	if(id){
		boxX = 3;
		boxY = 4;
		stageX = 3;
		stageY = 1;
	}else{
		boxX = 3;
		boxY = 4;
		stageX = 2;
		stageY = 1;
	}

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

void CatchRobo::assignLimit(Switch &limit,uint16_t address){
	this->limit[address] = &limit;
}

void CatchRobo::assignValve(CanValve &_valve){
	this->valve = &_valve;
}

void CatchRobo::cycle(){

	limit[0]->cycle();
	limit[1]->cycle();
	ps3->cycle();

	if(ps3->update){									//データ受信時
		ps3->update = 0;
		outputEnable = 1;
		serial->printf("%d,%d,%d,%d\n\r",ps3->data[PS3_HatLX],ps3->data[PS3_HatLY],ps3->data[PS3_HatRX],ps3->data[PS3_HatRY]);

		if(ps3->data[PS3_Select] == 1){
			motionType = Motion::origin;
		}

		if(ps3->data[PS3_Start] == 1){
			motionType = Motion::home;
		}

		if(motionType == Motion::shootingBox){			//シューティングボックス
			if(ps3->data[PS3_LEFT] == 1 && ps3->oldData[PS3_LEFT] == 0){
				if(id){
					if(boxX > 1)boxX--;
				}else{
					if(boxX < 3)boxX++;
				}
			}

			if(ps3->data[PS3_RIGHT] == 1 && ps3->oldData[PS3_RIGHT] == 0){
				if(id){
					if(boxX < 3)boxX++;
				}else{
					if(boxX > 1)boxX--;
				}
			}

			if(ps3->data[PS3_UP] == 1 && ps3->oldData[PS3_UP] == 0){
				if(boxY < 4)boxY++;
			}

			if(ps3->data[PS3_DOWN] == 1 && ps3->oldData[PS3_DOWN] == 0){
				if(boxY > 1)boxY--;
			}
		}

		if(motionType == Motion::workField){			//フィールド側
			if(ps3->data[PS3_LEFT] == 1 && ps3->oldData[PS3_LEFT] == 0){
				if(id){
					if(stageX > 1)stageX--;
				}else{
					if(stageX < 6)stageX++;
				}
			}

			if(ps3->data[PS3_RIGHT] == 1 && ps3->oldData[PS3_RIGHT] == 0){
				if(id){
					if(stageX > 1)stageX++;
				}else{
					if(stageX < 6)stageX--;
				}
			}

			if(ps3->data[PS3_UP] == 1 && ps3->oldData[PS3_UP] == 0){
				if(stageY < 4)stageY++;
			}

			if(ps3->data[PS3_DOWN] == 1 && ps3->oldData[PS3_DOWN] == 0){
				if(stageY > 1)stageY--;
			}
		}

		if(ps3->data[PS3_R1] == 1 && motionType != Motion::workField){
			motionType = Motion::workField;
		}

		if(ps3->data[PS3_L1] == 1 && motionType != Motion::shootingBox){
			motionType = Motion::shootingBox;
		}

		if(ps3->data[PS3_L2]){
			targetPosJ = 1200;
		}else if(ps3->data[PS3_R2]){
			targetPosJ = 800;
		}else{

		}

		handState = ps3->data[PS3_CIR];
		getState = ps3->data[PS3_CRO];


	}else if(millis() - ps3->lastReceiveTime > 100){	//タイムアウト
		//outputEnable = 0;
	}


	if(serial->available()){
		outputEnable = 1;
		serial->printf("\n\rR = %4d,J = %4d,\n\r\n\r",(int)(targetPosR*180/M_PI),(int)targetPosX);
		switch(serial->read()){
		case 'w':
			stageY++;
			boxY++;
			//if(targetPosR < 0)targetPosR += 0.02;
			break;
		case 's':
			stageY--;
			boxY--;
			//if(targetPosR > -(M_PI/2))targetPosR -= 0.02;
			break;
		case 'd':
			stageX++;
			boxX++;
			//if(targetPosX < 0)targetPosX += 20;
			break;
		case 'a':
			stageX--;
			boxX--;
			//if(targetPosX > -900)targetPosX -= 20;
			break;
		case 'r':
			if(map.isR){
				map.isR = 0;
			}else{
				map.isR = 1;
			}
			break;
		default:
			return;
		}
		//map.callWork(stageY,stageX);
		map.callShootingBox(boxY,boxX);
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
	case Motion::origin:		//初期位置合わせ

		if(readS() > 450){
			targetPosS = 430;
		}else if(limit[MotorR]->read() != 0){		//押されていない
				targetPosR = readR() + 0.01;	//1rad/s=57°/s
			}else if(limit[MotorR]->readStat == 0 && limit[MotorR]->read() == 0){	//押された
				enc[MotorR]->reset();
			}else{								//すでに押されている

			if(limit[MotorX]->read() != 0){		//押されていない
				if(id){							//右なので正､100mm/s
					targetPosX = readX() + 1;
				}else{
					targetPosX = readX() - 1;
				}
			}else if(limit[MotorX]->readStat == 0 && limit[MotorX]->read() == 0){	//押された
				enc[MotorX]->reset();
			}else{								//すでに押されている
				motionType = Motion::home;
				if(id){
					targetPosX = 50;
				}else{
					targetPosX = -50;
				}
			}
		}

		//armControl();
		motorControl();


		break;
	case Motion::home:						//スタート姿勢
		map.callWork(stageY,stageX);
		targetPosX = map.x;

		if(id){
			if(readX() > 40){
				targetPosS = 430;
				targetPosJ = 500;
				targetPosR = 1;
			}else{

			}
		}else{
			if(readX() < -40){
				targetPosS = 430;
				targetPosJ = 500;
				targetPosR = 1;
			}else{

			}
		}
		//armControl();
		motorControl();

		break;
	case Motion::workField:
		map.callWork(stageY,stageX);
		if(id){
			if(readX() > 0){	//フィールド入ってない
				targetPosX = map.x;
			}else{
				targetPosX = map.x;
				targetPosY = map.y;
				targetPosZ = map.z;
			}
		}else{
			if(readX() < 0){	//フィールド入ってない
				targetPosX = map.x;
			}else{
				targetPosX = map.x;
				targetPosY = map.y;
				targetPosZ = map.z;
			}
		}

		if(getState){
			targetPosZ -= 50;
		}else{
			if(handState == 0 && stageY > 1){		//ハンド開いてるので手前で退避
				targetPosY -= 50;
			}else{

			}
		}

		if(stageY == 1){
			if(id){
				if(stageX % 2 == 1){
					targetPosA = 0;
				}else{
					targetPosA = M_PI/2;	//逆向きなので反転させる
				}
			}else{
				if(stageX % 2 == 0){
					targetPosA = 0;
				}else{
					targetPosA = M_PI/2;	//逆向きなので反転させる
				}
			}

			targetPosP = readR();
		}else{
			targetPosA = 0;
			targetPosP = readR() + M_PI / 2;
		}
		targetPosX += (ps3->data[PS3_HatLX] - 0x40) * 0.5;
		targetPosY += (ps3->data[PS3_HatLY] - 0x40) * 0.5;
		targetPosA += (ps3->data[PS3_HatRX] - 0x40) * 0.5;

		armControl();
		motorControl();
		break;
	case Motion::shootingBox:

		map.callShootingBox(boxY,boxX);
		if(id){
			if(readX() < 0){	//フィールド入ってる
				targetPosX = map.x;
				targetPosY = map.y;
				targetPosZ = map.z;
			}else{
				targetPosX = map.x;
			}
		}else{
			if(readX() > 0){	//フィールド入ってる
				targetPosX = map.x;
				targetPosY = map.y;
				targetPosZ = map.z;
			}else{

				targetPosX = map.x;
			}
		}

		targetPosX += (ps3->data[PS3_HatLX] - 0x40) * 0.5;
		targetPosY += (ps3->data[PS3_HatLY] - 0x40) * 0.5;

		targetPosA = (ps3->data[PS3_HatRX] - 0x40) * 0.5;
		targetPosP = readR() + M_PI / 2;

		armControl();
		motorControl();
		break;
	}
}

void CatchRobo::armControl(){
	//ここではtergetX,Y,Zが指定されている
	float y_d = targetPosY + basePich_Offset_Y;
	float z_d = basePich_Offset_Z - targetPosZ;

	float virtualPitchR = atan2(y_d,z_d);
	float VirtualDistance = sqrt(y_d*y_d + z_d*z_d);

	targetPosS = VirtualDistance;//sqrt(VirtualDistance*VirtualDistance - basePich_Offset_D*basePich_Offset_D);
	targetPosR = virtualPitchR - atan2(basePich_Offset_D,VirtualDistance);						//ここは近似
	targetPosR = radian_to_deg(targetPosR);

	if(printCycleTime < millis()){
		printCycleTime += 500;
		//serial->printf("target X = %4d,S =%4d,R = %4d,J = %4d,",(int)targetPosX,(int)targetPosS,(int)targetPosR,(int)targetPosJ);
		//serial->printf("read X = %4d,S =%4d,R = %4d,J = %4d,\n\r",(int)readX(),(int)readS(),(int)(readR()*180/M_PI),(int)readJ());
	}
}

void CatchRobo::motorControl(){
	pid[MotorX].input(targetPosX,readX());
	pid[MotorR].input(targetPosR,readR());
	pid[MotorS].input(targetPosS,readS());
	pid[MotorJ].input(targetPosJ,readJ());

	for(int i=0;i<4;i++){
		output[i] = pid[i].outputF();
	}
	rFF = sin(readR()) * ((readS() - 430) / 1170) * MotorR_FF_S_Gain;
	output[MotorR] += rFF;

	/*output[0] = 0.1;
	output[1] = 0.1;
	output[2] = 0.1;
	output[3] = 0.1;*/

	outputEnable = 1;
	if(outputEnable){
		for(int i=0;i<4;i++){
			md[i]->duty(-output[i]);
		}
		md[ServoZ]->position(targetPosA);
		md[ServoP]->position(targetPosP);

		valve->write(0,handState);
	}else{
		for(int i=0;i<4;i++){
			md[i]->duty(0);
		}
	}
}

float CatchRobo::readR(){
	//10:25,2000*4cnt
	return ((float)enc[EncoderR]->read() / 8000 / 2.5 * 2 * M_PI);
}

float CatchRobo::readX(){
	return ((float)enc[EncoderX]->read() / 8000 * 50 * M_PI);
}

float CatchRobo::readS(){
	//1306cnt
	//1170mm + 430mm
	return ((float)pot[PotentioS]->read() * 1170 / 1260) + 430;
}

float CatchRobo::readJ(){
	return ((float)(4095 - pot[PotentioJ]->read()) * 1170 / 1260) + 500;
}
