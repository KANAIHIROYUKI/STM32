#include "CatchRoboMain.h"


void CatchRobo::setup(USART &serial,SBDBT &ps3,uint16_t _id){
	this->ps3 = &ps3;
	this->serial = &serial;
	id = _id;

	controlCycleTime = 10;	//制御周期

	pid[MotorX].setup(0.04,		0,	0.2);	//Iいらん
	pid[MotorR].setup(8,		0,	20);	//Iなくても良い気がする
	pid[MotorS].setup(0.005,	0,	0.001);	//I必要だけどガバい､FFのほうが良さそう
	pid[MotorJ].setup(0.005,	0,	0.001);
	pid[ServoP].setup(1.0,0,0);
	pid[ServoZ].setup(1.0,0,0);

	pid[MotorX].outLimit(-0.5,0.5);	//1
	pid[MotorR].outLimit(-0.2,0.5);
	pid[MotorS].outLimit(-0.7,0.7);	//0.9
	pid[MotorJ].outLimit(-0.2,0.2);

	pid[MotorS].errorLimitI(-0.2,0.2);
	pid[MotorR].errorLimitI(-0.02,0.02);

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

	valve->write(0,0);
	valve->write(1,0);

	//servoDuty[0] = 159 - ((M_PI/2)/M_PI)*50;
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
				offsetClear();
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
				offsetClear();
				swStat[PS3_RIGHT] = 1;
				if(id == 1 && boxX < 3)boxX++;
				if(id == 0 && boxX > 1)boxX--;
			}else if(ps3->data[PS3_RIGHT] == 0){
				swStat[PS3_RIGHT] = 0;
			}

			if(ps3->data[PS3_UP] == 1 && swStat[PS3_UP] == 0){
				offsetClear();
				swStat[PS3_UP] = 1;
				if(boxY < 4)boxY++;
			}else if(ps3->data[PS3_UP] == 0){
				swStat[PS3_UP] = 0;
			}

			if(ps3->data[PS3_DOWN] == 1 && swStat[PS3_DOWN] == 0){
				offsetClear();
				swStat[PS3_DOWN] = 1;
				if(boxY > 3)boxY--;
			}else if(ps3->data[PS3_DOWN] == 0){
				swStat[PS3_DOWN] = 0;
			}
		}

		if(motionType == Motion::workField || motionType == Motion::home){			//フィールド側
			if(ps3->data[PS3_LEFT] == 1 && swStat[PS3_LEFT] == 0){
				offsetClear();
				swStat[PS3_LEFT] = 1;
				if(id == 1 && stageX < 6)stageX++;
				if(id == 0 && stageX > 1)stageX--;
			}else if(ps3->data[PS3_LEFT] == 0){
				swStat[PS3_LEFT] = 0;
			}

			if(ps3->data[PS3_RIGHT] == 1 && swStat[PS3_RIGHT] == 0){
				offsetClear();
				swStat[PS3_RIGHT] = 1;
				if(id == 1 && stageX > 1)stageX--;
				if(id == 0 && stageX < 6)stageX++;
			}else if(ps3->data[PS3_RIGHT] == 0){
				swStat[PS3_RIGHT] = 0;
			}

			if(ps3->data[PS3_UP] == 1 && swStat[PS3_UP] == 0){
				offsetClear();
				swStat[PS3_UP] = 1;
				if(stageY < 4){
					if(stageY == 1){
						//ノーマル→共通でハンドが激突しないように制限つけている
						if(id == 1 && stageX % 2 == 0)stageY++;
						if(id == 0 && stageX % 2 == 1)stageY++;
					}else{
						//共通
						stageY++;
					}
				}
			}else if(ps3->data[PS3_UP] == 0){
				swStat[PS3_UP] = 0;
			}

			if(ps3->data[PS3_DOWN] == 1 && ps3->oldData[PS3_DOWN] == 0){
				offsetClear();
				swStat[PS3_DOWN] = 1;
				if(stageY > 1)stageY--;
			}else if(ps3->data[PS3_DOWN] == 0){
				swStat[PS3_DOWN] = 0;
			}
		}

		if(ps3->data[PS3_R1] == 1 && origin){
			offsetClear();
			if(id){
				if(motionType != Motion::home)motionType = Motion::shootingBox;
			}else{
				motionType = Motion::workField;
				if(stageY == 1){
					escape = 1;
				}else{
					escape = 0;
				}
			}
		}

		if(ps3->data[PS3_L1] == 1 && origin){
			offsetClear();
			if(id){
				motionType = Motion::workField;
				if(stageY == 1){
					escape = 1;
				}else{
					escape = 0;
				}
			}else{
				if(motionType != Motion::home)motionType = Motion::shootingBox;
			}
		}

		if(ps3->data[PS3_L2]){
			targetPosJ = 1700;
		}else if(ps3->data[PS3_R2]){
			targetPosJ = 1200;
		}else{

		}

		if(ps3->data[PS3_CIR] == 1){
			if(swStat[PS3_CIR] == 0){
				swStat[PS3_CIR] = 1;
				if(handState == 0){
					handState = 1;
					vData[0] = 0xFF;
					vData[1] = 0xFF;
					valve->can->send(0x280,2,vData);
				}else{
					handState = 0;
					vData[0] = 0xFF;
					vData[1] = 0x00;
					valve->can->send(0x280,2,vData);
				}
			}
		}else{
			swStat[PS3_CIR] = 0;
		}

		getState = ps3->data[PS3_CRO];
	}


	if(serial->available()){
		char serialData = serial->read();
		if(serialData == 'r'){
			IWDGSetup(1);
		}
	}

	if(millis() > cycleTime){			//制御周期
		cycleTime += controlCycleTime;
		controlCycle();
	}
}

void CatchRobo::controlCycle(){

	switch(motionType){
	case Motion::origin:/***********************************************************************************************初期化動作**********/

		//ピッチは常時格納位置
		//S縮ませる
		//R初期化+
		//
		//


		targetPosJ = 550;
		targetPosP = -M_PI/2;
		targetPosA = M_PI/2;

		if(readS() > 450 && originR == 0){
			targetPosS = 440;
		}else if(limit[MotorR]->readStat == 1 && limit[MotorR]->read() == 1 && originR == 0){	//押された
			enc[MotorR]->reset();
			originR = 1;
			serial->printf("R limit detect");
			limit[MotorX]->read();
		}else if(limit[MotorR]->read() == 0 && originR == 0){		//押されていない
			targetPosR = readR() - 0.015;	//1rad/s=57°/s
		}else{								//すでに押されている
			targetPosR = deg_to_radian(10);
			if(limit[MotorX]->readStat == 1 && limit[MotorX]->read() == 1){		//押された
				enc[MotorX]->reset();
				if(id){
					enc[MotorX]->write(-270*DistanceToCntX);
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
				handInternal = 1;
				origin = 1;
			}
		}

		//armControl();
		motorControl();


		break;
	case Motion::home:						//スタート姿勢
		if(stageX > 2)stageX = 2;
		map.callWork(stageY,stageX);
		targetPosX = map.x;
		targetPosS = 440;
		targetPosJ = 550;
		targetPosA = M_PI/2;

		if(readS() > 450){												//最初に縮める
			targetPosR = deg_to_radian(40);
			targetPosS = 440;
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

		//armControl();
		motorControl();

		break;
	case Motion::workField:/***********************************************************************************************ワークフィールド**********/
		map.callWork(stageY,stageX);

		targetPosX = map.x;
		targetPosY = map.y;
		targetPosZ = map.z;

		if((id == 1 && readX() > -100) || (id == 0 && readX() < 100)){
			armControl();
			targetPosS = 500;					//長さ固定
			targetPosR = deg_to_radian(60);		//仰角固定
			targetPosP = M_PI/2;
			//mapに従って動くのはXだけ
			motorControl();
			break;
		}

		if(readR() > deg_to_radian(43) && readS() < 550){		//角度ガバガバ､共通ワーク取りに行ってる時にバグらないようにSつき
			armControl();
			targetPosS = 500;
			targetPosR = deg_to_radian(30);
			targetPosP = M_PI/2;
			motorControl();
			break;
		}

		//初手のハンド引き出し動作
		if(readR() < deg_to_radian(40) && handInternal == 1){
			targetPosR = deg_to_radian(60);	//ある程度大きくしておかないとゲインの都合で遅い
			targetPosS = 435;
			targetPosP = -M_PI/2;
			motorControl();
			break;
		}else if(readR() > deg_to_radian(40) && handInternal == 1){
			handInternal = 0;
			break;
		}

		/********↑退避動作↓通常動作********/

		if(stageY == 1){
			if((id == 1 && stageX % 2 == 1) || (id == 0 && stageX % 2 == 0)){
				targetPosA = M_PI / 2;
			}else{
				targetPosA = -M_PI /2;
			}


			if(getState == 0){		//ボタン押してない時は上で待機
				targetPosZ += 100;
			}
			targetPosP = -readR();	//初期角度ぶんのオフセットはいらなかった(実験)､シューティングボックスに入れる時に都合がいいがち

		}else{											//共通ワーク

			if(getState){				//取りに行く時はz=-100,y=0
				targetPosZ -= 100;
				targetPosY -= 0;
			}else{
				if(handState){		//ハンド開閉
					//垂直に引き上げ
				}else{
					targetPosY -= 100;	//掴んでない時はz=0,y=-100
					targetPosZ -= 0;
				}
			}
			targetPosA = M_PI/2;
			targetPosP = -readR() + M_PI / 2;
		}

		offsetJog();

		armControl();

		//フィールド面へのハンドの衝突を防ぐ｡
		//ノーマル→共通のときは目標ピッチになるまで竿伸ばさない
		if(readR() < (targetPosR - deg_to_radian(1)))targetPosS = readS();
		//共通→ノーマルのときは先に竿を縮めてからピッチ変更する
		if(readS() > (targetPosS + 20))targetPosR = readR();

		motorControl();
		break;
	case Motion::shootingBox:/***********************************************************************************************シューティングボックス**********/
		map.callShootingBox(boxY,boxX);

		if(escape == 0){												//共通→ノーマル
			if(readR() > deg_to_radian(25)){
				//共通エリアから退避してノーマルエリアの上空へ行く｡
				targetPosY = 70;
				targetPosZ = 400;
				armControl();
				if(id){
					targetPosX = -100;
				}else{
					targetPosX = 100;
				}
				motorControl();
				break;
			}else{
				escape = 1;
			}
		}else if(escape == 1){											//ノーマル→シューティングボックス
			//ピッチめいっぱい上げる
			if(readR() < deg_to_radian(70))targetPosR = deg_to_radian(80);
			//竿も目一杯縮める
			if(readS() > 450)targetPosS = 430;
			//どっちもできたら完了してmap座標に従って動く
			if(readR() > deg_to_radian(70) && readS() < 550)escape = 2;

			targetPosP = M_PI / 2;
			targetPosA = M_PI / 2;
			if(id){
				targetPosX = -100;
			}else{
				targetPosX = 100;
			}
			motorControl();
			break;
		}

		targetPosY = map.y;
		targetPosZ = map.z;
		targetPosX = map.x;
		targetPosP = -readR() + M_PI / 2;// - deg_to_radian(15);


		if(id == 1 && readX() > 0)targetPosZ += 200;
		if(id == 0 && readX() < 0)targetPosZ += 200;


		if(getState)targetPosZ -= 100;


		//古い退避プログラム
		/*
		if(readS() > 590){	//アーム上がりきっていない
			if(id){
				targetPosX = -100;
			}else{
				targetPosX = 100;
			}
			targetPosZ += 200;
			targetPosP = M_PI/2;
		}else{

		}*/

		targetPosA = M_PI/2;
		offsetJog();
		//if(targetPosZ > 700) targetPosZ = 700;	//上げすぎると可動限界突破するため｡

		armControl();
		motorControl();
		break;
	}

	if(printCycleTime < millis()){
		printCycleTime += 200;
		serial->printf("rff = %d,",(int)(rFF*100));
		//serial->printf("s = %d,%d,",(int)(servoDuty[0] * 100),(int)(servoDuty[1] * 100));
		serial->printf("tar X= %4d,Y= %4d,Z= %4d,S= %4d,R= %4d,J= %4d,A= %4d,P= %4d",(int)targetPosX,(int)targetPosY,(int)targetPosZ,(int)targetPosS,(int)(targetPosR*180/M_PI),(int)targetPosJ,(int)(targetPosA*180/M_PI),(int)(targetPosP*180/M_PI));
		serial->printf("read X = %4d,S =%4d,R = %4d,J = %4d,",(int)readX(),(int)readS(),(int)(readR()*180/M_PI),(int)readJ());
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
}

void CatchRobo::motorControl(){
	if(targetPosS < 430)targetPosS =430;

	pid[MotorX].input(targetPosX,readX());
	pid[MotorR].input(targetPosR,readR());
	pid[MotorS].input(targetPosS,readS());
	pid[MotorJ].input(targetPosJ,readJ());

	for(int i=0;i<4;i++)output[i] = pid[i].outputF();

	if(motionType == Motion::origin){	//初期位置合わせにフィードフォワード項があると角度と補正量が合わずに動かないことがある･･･のだが手前にあるので動くっぽい｡
		rFF = 0;
	}

	rFF = sin(readR()) * (readS() / 1170) * MotorR_FF_S_Gain;
	sFF = -cos(readR()) * MotorS_FF_S_Gain;
	output[MotorR] += rFF;
	output[MotorS] += sFF;

	md[MotorX]->duty(output[MotorX]);	//かわロボ:+　本体:0
	md[MotorS]->duty(-output[MotorS]);	//かわろぼ:-　本体:+
	md[MotorJ]->duty(output[MotorJ]);	//かわろぼ:- 本体+

	if(id){
		md[MotorR]->duty(output[MotorR]);	//かわロボ:+ 本体:-
	}else{//右と左は反転している
		md[MotorR]->duty(-output[MotorR]);	//かわロボ:+ 本体:-
	}

	if(targetPosA < -M_PI/2)targetPosA = -M_PI/2;
	if(targetPosA > M_PI/2)targetPosA = M_PI/2;
	if(targetPosP < -M_PI/2)targetPosP = -M_PI/2;
	if(targetPosP > M_PI/2)targetPosP = M_PI/2;

	servoDuty[0] = 159 - (targetPosA/M_PI)*40;
	servoDuty[1] = 159 - (targetPosP/M_PI)*40;

	md[ServoZ]->position(servoDuty[0]);
	md[ServoP]->position(servoDuty[1]);
}

float CatchRobo::readR(){
	//10:25,2000*4cnt
	if(id){
		return ((float)enc[EncoderR]->read() / ENC_CNT / 2.5 * 2 * M_PI);
	}else{
		return (-(float)enc[EncoderR]->read() / ENC_CNT / 2.5 * 2 * M_PI);
	}
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

void CatchRobo::offsetClear(){
	for(int i=0;i<6;i++)offsetPos[i] = 0;
}

void CatchRobo::offsetJog(){
	//手動調節動作
	offsetPos[PosX] += (ps3->data[PS3_HatLX]) * 0.02;
	offsetPos[PosY] += (ps3->data[PS3_HatLY]) * -0.02;
	offsetPos[PosZ] += (ps3->data[PS3_HatRY]) * -0.02;
	offsetPos[PosA] += (ps3->data[PS3_HatRX]) * 0.0005;

	constrain(offsetPos[PosY],-200,200);
	constrain(offsetPos[PosY],-200,200);
	constrain(offsetPos[PosZ],-100,100);
	constrain(offsetPos[PosA],-M_PI/2,M_PI/2);

	targetPosX += offsetPos[PosX];
	targetPosY += offsetPos[PosY];
	targetPosZ += offsetPos[PosZ];
	targetPosA += offsetPos[PosA];
}
