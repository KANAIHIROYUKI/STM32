#include "CatchRoboMain.h"


void CatchRobo::setup(USART &serial,SBDBT &ps3,uint16_t _id){
	this->ps3 = &ps3;
	this->serial = &serial;
	id = _id;

	controlCycleTime = 10;	//制御周期

	pid[MotorX].setup(0.025,	0,	0.2);
	pid[MotorR].setup(2,		0,	2);
	pid[MotorS].setup(-0.01,		0,	-0.01);
	pid[MotorJ].setup(0.01,		0,	0.01);

	pid[ServoP].setup(1.0,0,0);
	pid[ServoZ].setup(1.0,0,0);

	pid[MotorX].outLimit(-0.2,0.2);
	pid[MotorR].outLimit(-0.2,0.2);
	pid[MotorS].outLimit(-0.2,0.2);
	pid[MotorJ].outLimit(-0.2,0.2);

	motionType = Motion::wait;
	origin = 0;
	originR = 0;
	handInternal = 0;
	handState = 0;

	if(id){
		boxX = 3;
		boxY = 4;
		stageX = 1;
		stageY = 1;
	}else{
		boxX = 3;
		boxY = 4;
		stageX = 1;
		stageY = 1;
	}

	map.setup(id);

	if(id){
		serial.printf("robot is right\n\r\n\r");
	}else{
		serial.printf("robot is left\n\r\n\r");
	}
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
		//serial->printf("%d,%d,%d,%d\n\r",ps3->data[PS3_HatLX],ps3->data[PS3_HatLY],ps3->data[PS3_HatRX],ps3->data[PS3_HatRY]);
		//serial->printf("%d,%d,%d,%d,%d\n\r",ps3->dataBuf[PS3_R1],ps3->dataBuf[PS3_L1],ps3->dataBuf[PS3_R2],ps3->dataBuf[PS3_L2],ps3->rxNum);
		//serial->printf("%d,%d,%d,%d\n\r",);
		//serial->printf("%d,%d\n\r",ps3->data[PS3_Select],ps3->data[PS3_Start]);

		if(ps3->data[PS3_Select] == 1){
			motionType = Motion::origin;
			originR = 0;
			origin = 0;
		}

		if(ps3->data[PS3_Start] == 1){
			motionType = Motion::home;
			handInternal = 0;
		}

		if(motionType == Motion::shootingBox){			//シューティングボックス
			if(ps3->data[PS3_LEFT] == 1 && swStat[PS3_LEFT] == 0){
				swStat[PS3_LEFT] = 1;
				if(id){
					if(boxX > 1)boxX--;
				}else{
					if(boxX < 3)boxX++;
				}
			}else if(ps3->data[PS3_LEFT] == 0){
				swStat[PS3_LEFT] = 0;
			}

			if(ps3->data[PS3_RIGHT] == 1 && swStat[PS3_RIGHT] == 0){
				swStat[PS3_RIGHT] = 1;
				if(id){
					if(boxX < 3)boxX++;
				}else{
					if(boxX > 1)boxX--;
				}
			}else if(ps3->data[PS3_RIGHT] == 0){
				swStat[PS3_RIGHT] = 0;
			}

			if(ps3->data[PS3_UP] == 1 && swStat[PS3_UP] == 0){
					swStat[PS3_UP] = 1;
					if(boxY < 4)boxY++;
			}else if(ps3->data[PS3_UP] == 0){
				swStat[PS3_UP] = 0;
			}

			if(ps3->data[PS3_DOWN] == 1 && swStat[PS3_DOWN] == 0){
				swStat[PS3_DOWN] = 1;
				if(boxY > 1)boxY--;
			}else if(ps3->data[PS3_DOWN] == 0){
				swStat[PS3_DOWN] = 0;
			}
		}

		if(motionType == Motion::workField || motionType == Motion::home){			//フィールド側
			if(ps3->data[PS3_LEFT] == 1 && swStat[PS3_LEFT] == 0){
				swStat[PS3_LEFT] = 1;
				if(id){
					if(stageX < 6)stageX++;
				}else{
					if(stageX > 1)stageX--;
				}
			}else if(ps3->data[PS3_LEFT] == 0){
				swStat[PS3_LEFT] = 0;
			}

			if(ps3->data[PS3_RIGHT] == 1 && swStat[PS3_RIGHT] == 0){
				swStat[PS3_RIGHT] = 1;
				if(id){
					if(stageX > 1)stageX--;
				}else{
					if(stageX < 6)stageX++;
				}
			}else if(ps3->data[PS3_RIGHT] == 0){
				swStat[PS3_RIGHT] = 0;
			}

			if(ps3->data[PS3_UP] == 1 && swStat[PS3_UP] == 0){
				swStat[PS3_UP] = 1;
				if(stageY < 4)stageY++;
			}else if(ps3->data[PS3_UP] == 0){
				swStat[PS3_UP] = 0;
			}

			if(ps3->data[PS3_DOWN] == 1 && ps3->oldData[PS3_DOWN] == 0){
				swStat[PS3_DOWN] = 1;
				if(stageY > 1)stageY--;
			}else if(ps3->data[PS3_DOWN] == 0){
				swStat[PS3_DOWN] = 0;
			}
		}

		if(ps3->data[PS3_R1] == 1 && motionType != Motion::workField && origin){
			if(id){
				motionType = Motion::shootingBox;
			}else{
				motionType = Motion::workField;
			}
		}

		if(ps3->data[PS3_L1] == 1 && motionType != Motion::shootingBox && origin){
			if(id){
				motionType = Motion::workField;
			}else{
				motionType = Motion::shootingBox;
			}
		}

		if(ps3->data[PS3_L2]){
			targetPosJ = 1200;
		}else if(ps3->data[PS3_R2]){
			targetPosJ = 800;
		}else{

		}

		if(ps3->data[PS3_CIR] == 1 && handState == 0){
			handState = 1;
			vData[0] = 0xFF;
			vData[1] = 0xFF;
			//valve->can->send(0x280,2,vData);
			valve->can->send(0x280,2,vData);
		}else if(ps3->data[PS3_CIR] == 0 && handState == 1){
			handState = 0;
			vData[0] = 0xFF;
			vData[1] = 0x00;
			//valve->can->send(0x280,2,vData);
			valve->can->send(0x280,2,vData);
		}
		getState = ps3->data[PS3_CRO];
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

	if(millis() > cycleTime){			//制御周期
		cycleTime += controlCycleTime;
		controlCycle();
	}
}

void CatchRobo::controlCycle(){

	switch(motionType){
	case Motion::origin:		//初期位置合わせ

		targetPosP = -M_PI/2;
		if(readS() > 450 && originR == 0){
			targetPosS = 430;
		}else if(limit[MotorR]->readStat == 1 && limit[MotorR]->read() == 1 && originR == 0){	//押された
			enc[MotorR]->reset();
			originR = 1;
			serial->printf("R limit detect");
			limit[MotorX]->read();
		}else if(limit[MotorR]->read() == 0 && originR == 0){		//押されていない
			targetPosR = readR() - 0.01;	//1rad/s=57°/s
		}else{								//すでに押されている
			targetPosR = deg_to_radian(30);
			if(limit[MotorX]->readStat == 1 && limit[MotorX]->read() == 1){		//押された
				enc[MotorX]->reset();
				if(id){
					enc[MotorX]->write((int64_t)((float)-270*DistanceToCntX));
				}else{
					enc[MotorX]->write(270*DistanceToCntX);
				}
				serial->printf("X limit detect");
			}else if(limit[MotorX]->read() == 0){		//押されていない
				//serial->printf("n");
				if(id){							//PD制御なのでてきとう､ゲイン0.025なのでduty0.25ぐらい
					targetPosX = readX() + 10;
				}else{
					targetPosX = readX() - 10;
				}
			}else{								//すでに押されている

				serial->printf("p");
				motionType = Motion::home;
				handInternal = 0;
				origin = 1;
				if(id){
					//targetPosX = -50;
				}else{
					//targetPosX = 50;
				}
			}
		}

		//armControl();
		motorControl();


		break;
	case Motion::home:						//スタート姿勢
		if(stageX > 2)stageX = 2;
		map.callWork(stageY,stageX);
		targetPosX = map.x;
		targetPosS = 430;
		targetPosJ = 500;

		if(readS() > 450){												//最初に縮める
			targetPosR = deg_to_radian(40);
			targetPosS = 430;
			targetPosP = -M_PI/2;
		}else{

			if(readR() < deg_to_radian(30) && handInternal == 0){		//アーム上げて格納
				targetPosR = deg_to_radian(40);
				targetPosP = -M_PI/2;
			}else if(readR() > deg_to_radian(30) && handInternal == 0){	//アームが一定角度以上になっているのを確認
				targetPosR = deg_to_radian(40);
				handInternal = 1;
			}else if(handInternal == 1){								//ロボットをスタートゾーンに収める
				targetPosR = 0;
			}
		}

		/*
		if(readR() < deg_to_radian(30)  && handInternal == 0){
			targetPosR = 40;
		}else if(readR() < deg_to_radian(30) && handInternal == 0){

		}

		if(readR() > deg_to_radian(25) && targetPosP != -M_PI/2){			//サーボに指令言っていなかった→入れる､
			targetPosP = -M_PI/2;
			//targetPosR = deg_to_radian(30);
		}else if(readR() > deg_to_radian(25) && targetPosP == -M_PI/2){
			//targetPosR = deg_to_radian(0);
		}else if(readR() < deg_to_radian(30)){
			if(id){
				if(readX() > 40){
					targetPosR = deg_to_radian(30);
				}else{

				}
			}else{
				if(readX() < -40){
					targetPosR = deg_to_radian(30);
				}else{

				}
			}
		}*/

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
					targetPosA = M_PI/2;
				}else{
					targetPosA = -M_PI/2;	//逆向きなので反転させる
				}
			}else{
				if(stageX % 2 == 0){
					targetPosA = M_PI/2;
				}else{
					targetPosA = -M_PI/2;	//逆向きなので反転させる
				}
			}

			targetPosP = -readR();
		}else{
			targetPosA = M_PI/2;
			targetPosP = -readR() + M_PI / 2;
		}
		targetPosX += (ps3->data[PS3_HatLX]) * 0.5;
		targetPosY += (ps3->data[PS3_HatLY]) * 0.5;
		targetPosA += (float)(ps3->data[PS3_HatRX]) / 20;

		if(readR() < deg_to_radian(20)){
			targetPosR = deg_to_radian(30);
			targetPosS = 430;
			targetPosP = -M_PI/2;
			motorControl();
			break;
		}

		armControl();
		motorControl();
		break;
	case Motion::shootingBox:
		map.callShootingBox(boxY,boxX);
		if(readR() < deg_to_radian(45)){	//フィールド入ってる
			targetPosY = map.y;
			targetPosZ = map.z;
		}else{
			targetPosY = map.y;
			targetPosZ = map.z;
			targetPosX = map.x;
		}

		targetPosX += (ps3->data[PS3_HatLX]) * 0.5;
		targetPosY += (ps3->data[PS3_HatLY]) * 0.5;

		targetPosA = (float)(ps3->data[PS3_HatRX]) / 20;
		targetPosP = -readR() + M_PI / 2;

		if(readR() < deg_to_radian(20)){
			targetPosR = deg_to_radian(30);
			targetPosS = 430;
			targetPosP = -M_PI/2;
			motorControl();
			break;
		}

		armControl();
		motorControl();
		break;
	}

	if(printCycleTime < millis()){
		printCycleTime += 200;
		serial->printf("rff = %d,",(int)(rFF*100));
		//serial->printf("s = %d,%d,",(int)(servoDuty[0] * 100),(int)(servoDuty[1] * 100));
		serial->printf("tar X= %4d,S= %4d,R= %4d,J= %4d,A= %4d,P= %4d",(int)targetPosX,(int)targetPosS,(int)(targetPosR*180/M_PI),(int)targetPosJ,(int)(targetPosA*180/M_PI),(int)(targetPosP*180/M_PI));
		serial->printf("read X = %4d,S =%4d,R = %4d,J = %4d,",(int)readX(),(int)readS(),(int)(readR()*180/M_PI),(int)readJ());
		//serial->printf("%d,%d,%d,%d",ps3->data[PS3_HatLX],ps3->data[PS3_HatLY],ps3->data[PS3_HatRX],ps3->data[PS3_HatRY]);
		serial->printf("m = %d,sx%d,sy%d,bs%d,by%d,v%d,\n\r",motionType,stageX,stageY,boxX,boxY,handState);
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
	//targetPosR = radian_to_deg(targetPosR);
}

void CatchRobo::motorControl(){
	pid[MotorX].input(targetPosX,readX());
	pid[MotorR].input(targetPosR,readR());
	pid[MotorS].input(targetPosS,readS());
	pid[MotorJ].input(targetPosJ,readJ());

	for(int i=0;i<4;i++){
		output[i] = pid[i].outputF();
	}
	rFF = sin(readR()) * (readS() / 1170) * MotorR_FF_S_Gain;
	output[MotorR] += rFF;

	/*output[0] = 0.1;
	output[1] = 0.1;
	output[2] = 0.1;
	output[3] = 0.1;*/

	outputEnable = 1;
	if(outputEnable){

		md[MotorX]->duty(output[MotorX]);
		md[MotorR]->duty(output[MotorR]);
		md[MotorS]->duty(output[MotorS]);
		md[MotorJ]->duty(output[MotorJ]);

		servoDuty[0] = 159 - (targetPosA/M_PI)*40;
		servoDuty[1] = 159 - (targetPosP/M_PI)*40;
		md[ServoZ]->position(servoDuty[0]);
		md[ServoP]->position(servoDuty[1]);


		//valve->write(0,handState);
		//valve->write(1,handState);
		//valve->write(2,handState);
		//valve->write(3,handState);

		//valve->write(4,handState);
		//valve->write(5,handState);
		//valve->write(6,handState);
		//valve->write(7,handState);

		//valve->can->send(0x280,2,vData);
	}else{
		for(int i=0;i<4;i++){
			md[i]->duty(0);
		}
	}
}

float CatchRobo::readR(){
	//10:25,2000*4cnt
	return ((float)enc[EncoderR]->read() / ENC_CNT / 2.5 * 2 * M_PI);
}

float CatchRobo::readX(){
	return (-(float)enc[EncoderX]->read() / ENC_CNT * 50 * M_PI);
}

float CatchRobo::readS(){
	//1306cnt
	//1170mm + 430mm
	return ((float)pot[PotentioS]->read() * 1170 / 1260) + 430;
}

float CatchRobo::readJ(){
	return ((float)(4095 - pot[PotentioJ]->read()) * 1170 / 1260) + 500;
}
