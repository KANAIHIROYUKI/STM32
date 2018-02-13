#include "KawaRoboMain.h"

void KawaRobo::cycle(){

	sbus->cycle();
	if((error | SA_LOST) == SA_LOST && error){		//Arduino側が起動していない時は送信停止する
		sa->sendRequest = 0;
	}
	sa->cycle();

	for(int i=0;i<6;i++){
		led[i]->cycle();
	}
	for(int i=0;i<4;i++){
		motor[i]->cycle();
	}

	sw[0]->cycle();
	sw[1]->cycle();

	armPotCycle();
	displayCycle();

	/**********************↑サイクル回すマン↓KRM****************************/
	if(millis() > SERIKA_TIME && serika && sa->update){//リセットかかった時になる｡普通に起動するとArduino側の方が起動遅いので鳴らない｡
		serika = 0;
		speakRequest = 10;						//アイドルマスターシンデレラガールズ
	}

	if(sa->update){
		sa->update = 0;
		saDataUpdate();
	}

	if(millis() - sbus->lastReceiveTime > SBUS_TIMEOUT_TOLERANCE){		//受信機と通信できない｡無条件
		if(mode != MODE_ERROR){
			motorDisable();
			serial->printf("\n\rmode = ERROR :: SBUS\n\r\n\r");
			led[1]->write(0);
			error |= SBUS_LOST;											//SBUS通信できていない
		}
		mode = MODE_ERROR;

	}else if(sbus->frameOk == 0){
		if(mode != MODE_DISCONNECT){
			serial->printf("\n\rmode = DISCONNECT\n\r\n\r");

			led[1]->interval(50);
			error &= 0xFFFF - SBUS_LOST;								//SBUS通信はできているが､送信機と未接続
		}
		mode = MODE_DISCONNECT;
	}else if(sbus->update){												//SBUS受信時
		sbus->update = 0;
		if(mode == MODE_DISCONNECT || mode == MODE_ERROR){				//これまでタイムアウトだったので｡

			led[1]->write(1);
			error &= 0xFFFF - SBUS_LOST;								//SBUS通信OK､送信機と接続
		}
		sbusDataUpdate();
	}

	if(SA_RECEIVE_OK){			//SerialArduinoのLEDとエラー処理
		if(saLedStat == 0){
			saLedStat = 1;
			led[0]->write(1);
			error &= 0xFFFF - SA_LOST;
		}
	}else{
		if(saLedStat == 1){
			saLedStat = 0;
			led[0]->write(0);
			error |= SA_LOST;
		}
	}

	if(error){
		if(error != oldError){
			if(error == Batt_UVL){	//下限電圧のみ
				led[3]->interval(1000);
			}else{
				led[3]->write(1);
			}

			oldError = error;
		}
	}else if(error != oldError){	//エラーなくなった
		oldError = error;
		led[3]->write(0);
	}

	if(millis() > controlCycleIntervalTime){	//制御周期
		controlCycleIntervalTime += 1;
		if(mode == MODE_RUN || mode == MODE_TEST){
			run();
		}else if(mode == MODE_STOP || mode == MODE_DISCONNECT || mode == MODE_ERROR){
			safety();
		}
	}

	if(speakRequest > 0){							//発声要求
		sa->write(1,speakRequest);
		serial->printf("\n\rserika = %d\n\r",speakRequest);
		sa->sendRequest = 0;
		speakRequest = -1;
		sa->sendEnded = 0;

		loopCycleCnt++;

	}else if(speakRequest == -1 && sa->sendEnded == 1){
		speakRequest = 0;
		sa->write(1,0);
		sa->write(2,0);
		sa->sendRequest = 0;
	}

	if(millis() > saSendTime){						//PowerENの送信間隔(連続で投げ続けるとArduinoが止まる)
		saSendTime += 50;
		sa->write(0,0);
	}
}

/*************************************************************************************/

void KawaRobo::displayCycle(){
	if(serial->available()){
		while(serial->available()){
			if(serial->peek() == 'p'){
				robotRGain *= 1.1;
				robotR.setup(robotRGain,0,0);
				serial->printf("\n\r\n\rr gain = %d\n\r\n\r",(int)(robotRGain*100));
			}else if(serial->peek() == 'm'){
				robotRGain *= 0.9;
				robotR.setup(robotRGain,0,0);
				serial->printf("\n\r\n\rr gain = %d\n\r\n\r",(int)(robotRGain*100));
			}else if(serial->peek() == 'o'){
				armPitchGain *= 1.1;
				armPitchGy.setup(armPitchGain,0,0);
				serial->printf("\n\r\n\rarm gain = %d\n\r\n\r",(int)(armPitchGain*100));
			}else if(serial->peek() == 'n'){
				armPitchGain *= 0.9;
				armPitchGy.setup(armPitchGain,0,0);
				serial->printf("\n\r\n\rarm gain = %d\n\r\n\r",(int)(armPitchGain*100));
			}{
				dispValue++;
			}
			serial->read();

		}
	}

	if(millis() > printTime){												//print周期
		printTime += PRINT_CYCLE;
		if(millis() - sbus->lastReceiveTime < SBUS_TIMEOUT_TOLERANCE){		//S.BUS受信できてる時
			if(getSelectPosition() < -0.9 && selectToggle == 0){
				selectToggle = 1;
				dispValue--;
				speakRequest = speakRequestNext;
				speakRequestNext--;
			}else if(getSelectPosition() > 0.9 && selectToggle == 0){
				selectToggle = 1;
				dispValue++;
				speakRequest = speakRequestNext;
				speakRequestNext++;
			}else{
				selectToggle = 0;
			}
		}


		if(sw[0]->readStat == 1 && sw[0]->read() == 0){
			dispValue++;
			//sa->write(2,30);
			//sa->write(1,dispValue + 1);
		}

		if(sw[1]->readStat == 1 && sw[1]->read() == 0){
			dispValue--;
			//sa->write(2,15);
		}

		serial->printf("er = %d,",error);
		switch(dispValue){
		case 0:
			dispValue = 6;

		case 1:
			serial->printf("sa ");
			for(int i=0;i<12;i++){
				serial->printf(",%5d",sa->read(i));
			}
			serial->printf(",g = %d",gravitatyAve.peek());
			break;

		case 2:
			serial->printf("sbus ");
			if(sbus->connect){
				serial->printf("connect");
			}else{
				serial->printf("lost");
			}
			if(sbus->frameOk){
				serial->printf("ok");
			}else{
				serial->printf("er");
			}
			for(int i=0;i<8;i++){
				//serial->printf("%d,",sbus->buffer[i]);
				serial->printf(",%5d",sbus->read(i));
			}
			serial->printf(",gy = %d",armPitchGy.output32());
			break;

		case 3:
			serial->printf("v = ,%d,mv,under voltage = %5d,adc2V = %4d",(int)battVoltage.peek(),(int)battUnderVoltage,(int)(adcToBattV*100));
			break;

		case 4:
			serial->printf("getRevPosition = %4d,robotR = ,%4d,targetR = ,%4d,robotPID = ,%4d",(int)(getRevPosition()*100),(int)robotR.outputF(),(int)robotTargetR.outputF(),robotPIDR.output32());
			break;

		case 5:
			serial->printf("getArmPosition = ,%4d",(int)(getArmPosition()*100));
			serial->printf("arm  analog");
			serial->printf("deg = ,%4d,PID out = ,%7d",(int)armDegree,armPID.output32());
			serial->printf("arm PID P = ,%7d,D = ,%4d",(int)(armPID.error*100),(int)(armPID.errorDiv*100));
			break;

		case 6:
			serial->printf("run = %4d,rev = %4d,arm = %4d,",(int)(getRunPosition()*100),(int)(getRevPosition()*100),(int)(getArmPosition()*100));
			serial->printf("motot out ");
			serial->printf(",L,%4d",(int)(motor[ML]->outDuty*100));
			serial->printf(",A,%4d",(int)(motor[MA]->outDuty*100));
			serial->printf(",R,%4d",(int)(motor[MR]->outDuty*100));
			break;

		default:
			dispValue = 1;
		}

		serial->printf("\n\r");
	}
}

/*************************************************************************************/

void KawaRobo::run(){
	float value = +(float)(((pot1Int/pot1Cnt)-2048)/4096);// - (pot2Int/pot2Cnt))/8191;
	potCnt = pot1Cnt + pot2Cnt;
	pot1Cnt = 0;
	pot1Int = 0;
	pot2Cnt = 0;
	pot2Int = 0;
	armDegree = -value * 270;

	if(armDegree > 30 && turnoverReturn == 1){	//転倒復帰
		turnoverReturn = 2;
		switch(loopCycleCnt %3){
		case 0:
			speakRequest = 57;//よいしょ
			break;
		case 1:
			speakRequest = 47;//うんしょ
			break;
		case 2:
			speakRequest = 43;//ジャーンプ！
			break;
		}
	}

	if(getArmPosition() < -0.5 && armDegree > 15 && millis() - armSpeakTime > 500 && turnoverReturn == 0){
		armSpeakTime = millis();
		switch(loopCycleCnt%8){			//｢はい！｣
		case 0:
			speakRequest = 111;
			break;
		case 1:
			speakRequest = 99;
			break;
		case 2:
			speakRequest = 100;
			break;
		case 3:
			speakRequest = 102;
			break;
		case 4:
			speakRequest = 105;
			break;
		case 5:
			speakRequest = 106;
			break;
		case 6:
			speakRequest = 107;
			break;
		case 7:
			speakRequest = 110;
			break;
		}
	}else if(armDegree > 30 && millis() - armSpeakTime < 500 && turnoverReturn == 0){
		armSpeakTime = millis();
	}

	output = 0;
	if(mode == MODE_TEST){
		output = getArmPosition();								//ジャイロ補正なし
	}else{
		output = getArmPosition();// + armPitchGy.outputF();		//ジャイロ補正あり
	}

	if(armDegree < ARM_BTM_DEG && output > 0.1){				//上限か下限に近づいていた場合､D制御で制動かける､duty0.1では制動モードに入らず動かせる｡
		armPID.measuredValue(-armDegree + ARM_BTM_DEG);
		output = armPID.outputF();
		motor[MA]->duty(output);						//
		motor[MS]->duty(output);

	}else if(armDegree > ARM_TOP_DEG && output < -0.1){
		armPID.measuredValue(-armDegree + ARM_TOP_DEG);
		output = armPID.outputF();
		motor[MA]->duty(output);
		motor[MS]->duty(output);

	}else{
		motor[MA]->duty(output);						//上限でも下限でもないので完全手動
		motor[MS]->duty(output);

	}
	/*
	if(millis() - modeChangeTime < 500 && modeChangeTime != 0){
		serial->printf("\n\r%d",(int)(output*100));
	}*/

	if(mode == MODE_TEST){				//手動モード
		motor[MR]->duty(+getRunPosition() + getRevPosition());
		motor[ML]->duty(-getRunPosition() + getRevPosition());
		return;
	}
																	//制御モード
	robotTargetR.measuredValue(getRevPosition());
	robotPIDR.measuredValue(robotR.outputF() - robotTargetR.outputF());
	motor[MR]->duty(+getRunPosition() - robotPIDR.outputF());
	motor[ML]->duty(-getRunPosition() - robotPIDR.outputF());

	//armCurrent.measuredValue((motor[1]->outDuty*14) - 0.1);
}

void KawaRobo::safety(){
	float value = +(float)(((pot1Int/pot1Cnt)-2048)/4096);// - (pot2Int/pot2Cnt))/8191;
	potCnt = pot1Cnt + pot2Cnt;
	pot1Cnt = 0;
	pot1Int = 0;
	pot2Cnt = 0;
	pot2Int = 0;
	armDegree = -value * 270;

	armPID.measuredValue(-armDegree + ARM_BTM_DEG);
	robotTargetR.measuredValue(getRevPosition());
	robotPIDR.measuredValue(robotR.outputF() - robotTargetR.outputF());

	motorDisable();
}



/*************************************************************************************/

//SerialArduino受信した時の処理
void KawaRobo::saDataUpdate(){
	robotR.measuredValue(-(float)(sa->read(2))/39200);
	if(abs(sa->read(1)) > 100){
		armPitchGy.measuredValue((float)sa->read(1)/39200);
	}

	if(battVoltage.valueCnt > 500){ 			//100回平均
		float val = battVoltage.read();
		battVoltage.reset();

		battVoltage.valueCnt = 10;				//初期10回分を前回の平均で入れる
		battVoltage.stack(val*10);
	}
	battVoltage.stack((sa->read(3) * adcToBattV));

	gravitatyAve.stack(sa->read(10)/10);
	if(gravitatyAve.valueCnt > 50){
		if(gravitatyAve.peek() < -1000){		//裏返った時｡行ってもたぶん-1500ぐらい
			if(turnoverReturn == 0){
				switch(loopCycleCnt%9){
				case 0:
					speakRequest = 58;//ううーん
					break;
				case 1:
					speakRequest = 49;//恥ずかしいです
					break;
				case 2:
					speakRequest = 54;//見られちゃいました
					break;
				case 3:
					speakRequest = 94;//↓はっ
					break;
				case 4:
					speakRequest = 96;
					break;
				case 5:
					speakRequest = 98;
					break;
				case 6:
					speakRequest = 101;
					break;
				case 7:
					speakRequest = 109;
					break;
				case 8:
					speakRequest = 112;//↑はっ
					break;
				}
				turnoverReturn = 1;
			}
		}else if(gravitatyAve.peek() > -100 && turnoverReturn != 0){	//反転してないよ
			switch(loopCycleCnt%4){//復帰した時
			case 0:
				speakRequest = 103;//です！
				break;
			case 1:
				speakRequest = 76;//やりました！
				break;
			case 2:
				speakRequest = 70;//じゃじゃーん！
				break;
			case 3:
				speakRequest = 46;//できました！
				break;//38?
			}
			turnoverReturn = 0;
		}
		gravitatyAve.read();
	}


}

//S.BUS受信した時の処理
void KawaRobo::sbusDataUpdate(){
	//serial->printf("%d\n\r",sbus->read(2));
	if(SA_RECEIVE_OK){
		if(battVoltage.peek() > battUnderVoltage){		//Arduinoからの受信がある､かつ電圧OK
			battUnderVoltage = BATT_UNDER_LIMIT;
			error &= 0xFFFF - Batt_UVL;
			if(getTogglePosition(TOGGLE_CONTROL_MODE) > 0.5){				//プロポのトグルスイッチの状態によって動作モードを変更する
				if(mode != MODE_RUN){

					mode = MODE_RUN;

					switch(loopCycleCnt % 5){
					case 0:
						speakRequest = 18;				//わたしにいろんなこと､教えて下さいね！
						break;
					case 1:
						speakRequest = 14;				//一生懸命練習したので応援してくださいね
						break;
					case 2:
						speakRequest = 36;				//いいところ見せちゃいますね
						break;
					case 3:
						speakRequest = 30;				//わたし､やってみたいです
						break;
					case 4:
						speakRequest = 52;				//よろしくお願いしますね！
						break;
					}


					motorEnable();
					serial->printf("\n\rmode = RUN\n\r\n\r");
				}

			}else if(getTogglePosition(TOGGLE_CONTROL_MODE) < 0.5){
				if(mode != MODE_TEST){

					if(mode == MODE_RUN){		//RUN→TEST
						switch((int)getTogglePosition(TOGGLE_SITUATION)){
						case -1:				//負け
							switch(loopCycleCnt % 5){
							case 0:
								speakRequest = 42;			//ありがとうございました！
								break;
							case 1:
								speakRequest = 44;			//とっても楽しかったです！
								break;
							case 2:
								speakRequest = 55;			//ありがとうございます！
								break;
							case 3:
								speakRequest = 60;			//ありがとうございます！
								break;
							case 4:
								speakRequest = 82;			//すごいです！
								break;
							}
							break;
						case 0:				//それ以外､仕切り直しを想定して何も言わない

							break;
						case 1:				//勝ち
							switch(loopCycleCnt % 4){
							case 0:
								speakRequest = 81;			//嬉しいです！
								break;
							case 1:
								speakRequest = 17;			//今の､とっても上手にできましたよね！
								break;
							case 2:
								speakRequest = 26;			//わたし､とってもうれしいです！
								break;
							case 3:
								speakRequest = 67;			//やったー！
								break;
							}
							break;
						}
					}else{						//STOP→TEST
						switch(loopCycleCnt % 4){
						case 0:
							speakRequest = 79;				//どうですか？
							break;
						case 1:
							speakRequest = 89;				//えへへ
							break;
						case 2:
							speakRequest = 80;				//えっへへ
							break;
						case 3:
							speakRequest = 45;				//お仕事体験です！
							break;
						}
					}

					mode = MODE_TEST;

					motorEnable();
					serial->printf("\n\rmode = TEST\n\r\n\r");
				}

			}else{
				if(mode != MODE_STOP){
					mode = MODE_STOP;

					if(mode == MODE_TEST || mode == MODE_RUN){
						switch(millis() % 4){
						case 0:
							speakRequest = 20;			//わたしのこと､褒めてくれますか？
							break;
						case 1:
							speakRequest = 68;			//お疲れ様です！
							break;
						case 2:
							speakRequest = 86;			//たのしいです
							break;
						case 3:
							speakRequest = 15;			//はぁ～､なんとかできました
							break;
						}
					}

					motorDisable();
					serial->printf("\n\rmode = STOP\n\r\n\r");
				}
			}
		}else{
			if(mode != MODE_STOP){						//電圧不足

				if(loopCycleCnt%2){
					speakRequest = 13;//怒っちゃいますよ
				}else{
					speakRequest = 73;//だめですよ！
				}
				motorDisable();
				serial->printf("\n\rbatt under voltage\n\rmode = STOP\n\r\n\r");

				error |= Batt_UVL;
				battUnderVoltage = BATT_UNDER_LIMIT + BATT_UNDER_HYS;
			}
		}
	}else{
		if(mode != MODE_ERROR){
			mode = MODE_ERROR;

			motorDisable();

			serial->printf("\n\rmode = ERROR :: SerialArduino\n\r\n\r");
		}
	}

	if(toggleStat[TOGGLE_POWER_LIMIT] != (int)getTogglePosition(TOGGLE_POWER_LIMIT)){		//出力リミッタ切り替え
		if(getTogglePosition(TOGGLE_POWER_LIMIT) > 0.1){
			motor[MR]->dutyLimit(0.99);
			motor[ML]->dutyLimit(0.99);
			switch(loopCycleCnt % 2){
			case 0:
				speakRequest = 35;						//一生懸命がんばります！
				break;
			case 1:
				speakRequest = 5;						//どんなことも諦めちゃいけないって､プロデューサーさんが行ってました！
				break;
			}
		}else if(getTogglePosition(TOGGLE_POWER_LIMIT) > -0.1){
			motor[MR]->dutyLimit(0.7);
			motor[ML]->dutyLimit(0.7);
			switch(loopCycleCnt % 2){
			case 0:
				speakRequest = 11;						//応援してもらうと,なんでもできちゃう気がします！
				break;
			case 1:
				speakRequest = 21;						//えいえいおー！
				break;
			case 2:
				speakRequest = 56;						//ファイト～！
				break;
			}

		}else{
			motor[MR]->dutyLimit(0.4);
			motor[ML]->dutyLimit(0.4);
			switch(loopCycleCnt % 3){
			case 0:
				speakRequest = 39;						//一生懸命やりました！
				break;
			case 1:
				speakRequest = 15;						//はぁ～､なんとかできました
				break;
			case 2:
				speakRequest = 33;						//わたし､がんばりました！
				break;
			}
		}


		if(toggleStat[TOGGLE_POWER_LIMIT] == 2)speakRequest = 0;							//初回設定
		toggleStat[TOGGLE_POWER_LIMIT] = (int)getTogglePosition(TOGGLE_POWER_LIMIT);
	}

	if(toggleStat[TOGGLE_VOICE] != (int)getTogglePosition(TOGGLE_VOICE)){
		toggleStat[TOGGLE_VOICE] = (int)getTogglePosition(TOGGLE_VOICE);

		switch((int)getTogglePosition(TOGGLE_SITUATION)){
		case -1:				//不利
			switch(loopCycleCnt % 2){
			case 0:
				speakRequest = 28;			//あはは！くすぐったいよ！
				break;
			case 1:
				speakRequest = 83;			//えぇーっ！
				break;
			}
			break;
		case 0:				//無
			switch(loopCycleCnt % 3){
			case 0:
				speakRequest = 9;			//あの～､どうやって食べたら良いんでしょうか
				break;
			case 1:
				speakRequest = 25;			//髪がぴょこぴょこしちゃいます
				break;
			case 2:
				speakRequest = 16;			//じっと見てると眠くなっちゃいそう
				break;
			}
			break;
		case 1:				//優勢
			switch(loopCycleCnt % 2){
			case 0:
				speakRequest = 72;			//決めちゃいますね！
				break;
			case 1:
				speakRequest = 19;			//お空に向かって､そーれっ！
				break;
			}
			break;
		}
	}
}

//制御周期関係なくぶんまわし
void KawaRobo::armPotCycle(){
	if(analog[0]->converted()){
		pot1Int += analog[0]->peek();
		pot1Cnt++;
		analog[0]->start();
	}

	if(analog[1]->converted()){
		pot2Int += analog[1]->peek();
		pot2Cnt++;
		analog[1]->start();
	}
}


/***********************************初期設定関数群**************************************************/

void KawaRobo::setup(USART &serialSet,SBUS &sbusSet,SerialArduino &saSet,NCP5359 &motor0,NCP5359 &motor1,NCP5359 &motor2,NCP5359 &motor3,GPIO &motorEN){
	this->serial = &serialSet;
	this->sbus = &sbusSet;
	this->sa   = &saSet;

	this->motor[0] = &motor0;
	this->motor[1] = &motor1;
	this->motor[2] = &motor2;
	this->motor[3] = &motor3;
	this->motorEn = &motorEN;

	motor[MA]->dutyLimit(0.99);
	motor[MS]->dutyLimit(0.99);
	motor[ML]->dutyLimit(0.4);
	motor[MR]->dutyLimit(0.4);

	printValueSelect = 0;
	battUnderVoltage = BATT_UNDER_LIMIT;

	dispValue = 1;
	speakRequest = 0;
	speakRequestNext = 0;
	serika = 1;
	loopCycleCnt = 0;


	turnoverReturn = 0;
	armOffsetDuty = 0;
	saLedStat = 0;

	saSendTime = millis();
	printTime = millis();

	mode = MODE_DISCONNECT;
	error = NONE;
	/*
	robotR.setup(0,1,0);			//ジャイロによる角度			単位度
	robotTargetR.setup(0,0.1,0);	//プロポによる角度設定		単位度
	robotPIDR.setup(0.01,0,0);		//制御用コントローラ､ゲイン1で1deg→100%になる
	robotPIDR.outLimit(-1,1);//*/


	armPitchGain = 1;
	armPitchGy.setup(armPitchGain,0,0);			//ジャイロによる角度積分

	robotRGain = 0.5;
	robotR.setup(robotRGain,0,0);			//ジャイロによる速度			単位度
	robotTargetR.setup(1,0,0);		//プロポによる速度設定		単位度
	robotPIDR.setup(1,0,0);		//制御用コントローラ､ゲイン1で1deg→100%になる
	robotPIDR.outLimit(-1,1);

	armPID.setup(0,0,-10);		//機構限界制動用
	armPID.outLimit(-0.1,0.1);

	armCurrent.setup(0,1,0);
	armCurrent.errorLimitInt(0,100);

	toggleStat[TOGGLE_CONTROL_MODE] = 2;
	toggleStat[TOGGLE_POWER_LIMIT]  = 2;
	//cyclleまだ回ってねえよ
	//toggleStat[TOGGLE_CONTROL_MODE] = (int)getTogglePosition(TOGGLE_CONTROL_MODE);
	//toggleStat[TOGGLE_POWER_LIMIT]  = (int)getTogglePosition(TOGGLE_POWER_LIMIT);
}

void KawaRobo::uiSetup(Switch &sw0,Switch &sw1,LED &led0,LED &led1,LED &led2,LED &led3,LED &led4,LED &led5){
	this->sw[0] = &sw0;
	this->sw[1] = &sw1;

	this->led[0] = &led0;
	this->led[1] = &led1;
	this->led[2] = &led2;
	this->led[3] = &led3;
	this->led[4] = &led4;
	this->led[5] = &led5;
}

void KawaRobo::sensorSetup(ADC &adc0,ADC &adc1,ADC &adc2,ADC &adc3,float _adcToBatt){
	this->analog[0] = &adc0;
	this->analog[1] = &adc1;
	this->analog[2] = &adc2;
	this->analog[3] = &adc3;
	adcToBattV = _adcToBatt;
}

/*************************************************************************************/

void KawaRobo::motorDisable(){

	motorEn->write(0);		//motor disable
	led[2]->write(0);
	for(int i=0;i<4;i++){
		motor[i]->duty(0);
	}
}

void KawaRobo::motorEnable(){

	for(int i=0;i<4;i++){
		motor[i]->duty(0);
	}
	motorEn->write(1);		//motor enable
	led[2]->write(1);

	return;
	robotPIDR.clear();		//いらん動きしないようにPIDコントローラーもリセット
	robotTargetR.clear();	//目標角度を0にする､現在の角度と目標角度の差を無くす
	robotR.clear();			//ジャイロ角度の積分値をリセット､現在角度を0度とする

	armPID.clear();			//いらん動きしないようにPIDコントローラーもリセット
	armPitchGy.clear();
}

/*************************************************************************************/

float KawaRobo::getRunPosition(){
	return (float)getStickPosition(CHANNEL_RUN)/STICK_MAX;
}

float KawaRobo::getArmPosition(){
	return (float)getStickPosition(CHANNEL_ARM)/(STICK_MAX);

	/*if(sbus->read(CHANNEL_ARM) < 802){

	}else if(sbus->read(CHANNEL_ARM) < 1242){
		return (float)getStickPosition(CHANNEL_ARM,1024)/(STICK_MAX/3);
	}else{
		return (float)getStickPosition(CHANNEL_ARM,1464)/(STICK_MAX/3);
	}*/
}

float KawaRobo::getRevPosition(){
	return (float)getStickPosition(CHANNEL_REV)/STICK_MAX;
}

float KawaRobo::getSelectPosition(){
	return (float)getStickPosition(CHANNEL_SEL)/STICK_MAX;
}

float KawaRobo::getTogglePosition(uint16_t num){
	switch(num){
	case 0:
		if(sbus->read(CHANNEL_TOGGLE0) < 500){
			return 1.0;
		}else if(sbus->read(CHANNEL_TOGGLE0) < 1500){
			return 0;
		}else{
			return -1.0;
		}
		break;

	case 1:
		if(sbus->read(CHANNEL_TOGGLE1) < 500){
			return 1.0;
		}else if(sbus->read(CHANNEL_TOGGLE1) < 1500){
			return 0;
		}else{
			return -1.0;
		}
		break;

	case 2:
		if(sbus->read(CHANNEL_TOGGLE2) < 500){
			return 1.0;
		}else if(sbus->read(CHANNEL_TOGGLE2) < 1500){
			return 0;
		}else{
			return -1.0;
		}
		break;
	case 3:
		if(sbus->read(CHANNEL_TOGGLE3) < 500){
			return 1.0;
		}else{
			return -1.0;
		}
		break;

	default:
		return 0;
	}
}

float KawaRobo::getVolumePosition(){
	return 0;
	//return (float)(sbus->read(CHANNEL_VOLUME)-1024)/880;
}

int KawaRobo::getStickPosition(uint16_t channel,uint16_t offset){
	int data = sbus->read(channel) - offset;

	if(data >= 0){
		if(data < STICK_TOLERANCE){
			data = 0;
		}else{
			data -= STICK_TOLERANCE;
		}
	}else{
		if(data > -STICK_TOLERANCE){
			data = 0;
		}else{
			data += STICK_TOLERANCE;
		}
	}

	if(data > STICK_MAX)data = STICK_MAX;
	if(data < -STICK_MAX)data = -STICK_MAX;

	return data;
}
