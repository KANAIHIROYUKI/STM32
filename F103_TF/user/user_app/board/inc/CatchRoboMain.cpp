#include "CatchRoboMain.h"


void CatchRobo::setup(USART &serial,SBDBT &ps3,uint16_t _id){
	this->ps3 = &ps3;
	this->serial = &serial;
	id = _id;

	controlCycleTime = 10;	//制御周期

	pid[MotorR].setup(8,		0,	20);	//Iなくても良い気がする
	//pid[MotorJ].setup(0.005,	0,	0.001);
	pid[ServoP].setup(1.0,0,0);
	pid[ServoZ].setup(1.0,0,0);

	pid[MotorR].outLimit(-0.2,0.5);
	pid[MotorS].outLimit(-0.7,0.7);	//0.9

	pid[MotorS].errorLimitI(-0.2,0.2);
	pid[MotorR].errorLimitI(-0.02,0.02);

	motionType = Motion::wait;
	origin = 0;

	serial.printf("robot is right\n\r\n\r");

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
		targetPosP = -M_PI/2;
		targetPosA = M_PI/2;
		//armControl();
		motorControl();


		break;
	}

	if(printCycleTime < millis()){
		printCycleTime += 200;
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

	pid[MotorR].input(targetPosR,readR());
	pid[MotorS].input(targetPosS,readS());

	for(int i=0;i<4;i++)output[i] = pid[i].outputF();

	if(motionType == Motion::origin){	//初期位置合わせにフィードフォワード項があると角度と補正量が合わずに動かないことがある･･･のだが手前にあるので動くっぽい｡
		rFF = 0;
	}

	rFF = sin(readR()) * (readS() / 1170) * MotorR_FF_S_Gain;
	sFF = -cos(readR()) * MotorS_FF_S_Gain;
	output[MotorR] += rFF;
	output[MotorS] += sFF;

	//md[MotorX]->duty(output[MotorX]);	//かわロボ:+　本体:0
	md[MotorS]->duty(-output[MotorS]);	//かわろぼ:-　本体:+
	//md[MotorJ]->duty(output[MotorJ]);	//かわろぼ:- 本体+

	md[MotorR]->duty(output[MotorR]);	//かわロボ:+ 本体:-

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
	return ((float)enc[EncoderR]->read() / ENC_CNT / 2.5 * 2 * M_PI);
}

float CatchRobo::readS(){
	//1306cnt
	//1170mm + 430mm
	return ((float)pot[PotentioS]->read() * 1170 / 1260) + 430;
}

void CatchRobo::offsetClear(){
	for(int i=0;i<6;i++)offsetPos[i] = 0;
}
