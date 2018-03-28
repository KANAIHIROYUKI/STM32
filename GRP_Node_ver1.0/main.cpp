#include "main.hpp"

int main(void)
{
	setup();
	motorEn.write(1);

	led[0].interval(500);

	servoSet[0] = 0;	//シェイクロック
	servoSet[1] = 0;	//シェイカー保持ロック
	servoSet[2] = 180;	//シェイカーグリップ
	servoSet[3] = 90;	//チルト

	servoSet[4] = 90;	//↓予備
	servoSet[5] = 90;
	servoSet[6] = 180;
	servoSet[7] = 180;

	servoDef[0] = 165;	//シェイクロック
	servoDef[1] = 170;	//シェイカー保持ロック
	servoDef[2] = 90;	//シェイカーグリップ
	servoDef[3] = 0;	//チルト

	servoDef[4] = 90;	//↓予備
	servoDef[5] = 90;
	servoDef[6] = 180;
	servoDef[7] = 180;

	for(int i=0;i<8;i++){
		sa.write(i + 4,servoDef[i]);
	}

    while(1){
    	if(millis() - intervalTime > 200){
    		intervalTime = millis();
    		//serial.printf("gy = %5d,",sa.read(2));
    		if(individual.read()){
    			serial.printf("%d,",controlMode);
    			serial.printf("%+6d,,",sa.read(2));	//ジャイロ

        		for(int i=0;i<12;i++){
        			//serial.printf("%+4d,",ps3.read(i));
        			//serial.printf("m%d = %d%%",i,(int)(mc.out[i]*100));
        		}
        		serial.printf("s=");
        		for(int i=0;i<5;i++){
        			serial.printf("%3d,",servoDeg[i]);
        		}
        		serial.printf("m=");
        		for(int i=0;i<4;i++){
        			serial.printf("%+4d,",(int)(motor[i].outDuty*100));
        		}
        		serial.printf("can=%+4d,%+4d,%+4d",uchar4_to_int(tx[0].data),uchar4_to_int(tx[1].data),uchar4_to_int(tx[2].data));
    		}else{

    			serial.printf("%d,",(int)can.lastReceiveTime);
    			for(int i=0;i<3;i++){
    				serial.printf("%d,",uchar4_to_int(rx[i].data));
    			}
        		for(int i=0;i<4;i++){
        			//serial.printf("%d,",ps3.read(i));
        			serial.printf("m%d = %4d%%",i,(int)(mc.out[i]*100));
        		}
    		}

    		serial.printf("\n\r");
    		//serial.printf("x = %d,y = %d,rad = %d\n\r",uchar4_to_int(rx[0].data),uchar4_to_int(rx[1].data),uchar4_to_int(rx[2].data));
    	}

    	if(individual.read()){
        	if(ps3.update){
        		ps3.update = 0;

        		switch(controlMode){
        		case 0://自走
        			int_to_uchar4(tx[0].data,hatTls(ps3.read(HAT_RX)));	//x
            		int_to_uchar4(tx[1].data,hatTls(ps3.read(HAT_RY)));	//y
            		int_to_uchar4(tx[2].data,hatTls(ps3.read(HAT_LX)));	//rad

            		tx[0].send();
            		tx[1].send();
            		tx[2].send();
            		speakSwitchEvent(BUTTON_R1,'<',2);//おまたせしました
            		speakSwitchEvent(BUTTON_L1,'2',2);//いらっしゃい
        			break;
        		case 1://シェイク→ロック解除→持ち上げまで
        			motor[MOTOR_SHAKE].duty((float)hatTls(ps3.read(HAT_RY))/100);	//シェイカー
        			motor[MOTOR_TILT].duty((float)hatTls(ps3.read(HAT_LY))/100);	//巻取り

        			servoSwitchEvent(BUTTON_TR,SERVO_TILT_LOCK);					//ロック解除

        			speakSwitchEvent(BUTTON_L1,'F',2);//まだまだ
        			speakSwitchEvent(BUTTON_R1,'4',8);//えいっ､それっ
        			break;

        		case 2://ロック解除→シェイク機構退場
        			motor[MOTOR_SHAKE].duty((float)hatTls(ps3.read(HAT_RY))/100);	//シェイカー持ち上げ用
        			servoSwitchEvent(BUTTON_SQ,SERVO_HOLDER_LOCK);					//シェイカー放す
        			speakSwitchEvent(BUTTON_L1,'>',4);//完璧
        			speakSwitchEvent(BUTTON_R1,'H',4);//美味しいカクテルにしますね
        			break;

        		case 3://シェイカー保持→注ぎ角度まで移動
        			servoSwitchEvent(BUTTON_TR,SERVO_GRIP);							//シェイカー掴む
        			motor[MOTOR_PICKUP].duty((float)hatTls(ps3.read(HAT_LY))/100);	//保持ハンド

        			ave.stack(hatTls(ps3.read(HAT_RY)));							//保持角度調節
        			if(ave.valueCnt > 10){
        				servoDeg[SERVO_POUT] -= ave.read()/5;
        				servoDeg[SERVO_POUT] = constrain(servoDeg[SERVO_POUT],0,180);
        				sa.write(SERVO_POUT+4,servoDeg[SERVO_POUT]);
            			ave.reset();
        			}
        			speakSwitchEvent(BUTTON_R1,'<',2);//おまたせしました
        			speakSwitchEvent(BUTTON_L1,'D',2);//またおこしください
        			break;

        		case 4://注ぐ+押し出し
        			ave.stack(hatTls(ps3.read(HAT_RY)));							//保持角度調節(注ぎ用)
        			if(ave.valueCnt > 10){
        				servoDeg[SERVO_POUT] -= ave.read()/5;
        				servoDeg[SERVO_POUT] = constrain(servoDeg[SERVO_POUT],0,180);
        				sa.write(SERVO_POUT+4,servoDeg[SERVO_POUT]);
            			ave.reset();
        			}
        			motor[MOTOR_PICKUP].duty((float)hatTls(ps3.read(HAT_LY))/100);	//保持ハンド
        			motor[MOTOR_PUSH].duty((float)hatTls(ps3.read(HAT_LX))/100);	//スライダ移動用
        			speakSwitchEvent(BUTTON_R1,'0',2);//あちらのお客様からです
        			speakSwitchEvent(BUTTON_L1,'B',2);//どうぞめしあがれ

        			break;
        		default:
        			controlMode = 4;
        		}

        		/*****************************************************/

    			if(ps3.read(BUTTON_CI) < 50 && swStat[BUTTON_CI] == 0){			//モード選択
    				swStat[BUTTON_CI] = 1;	//押された
    				controlMode++;
    			}else if(ps3.read(BUTTON_CI) > 90 && swStat[BUTTON_CI] == 1){
    				swStat[BUTTON_CI] = 0;	//離された
    			}

    			if(ps3.read(BUTTON_CR) < 50 && swStat[BUTTON_CR] == 0){
    				swStat[BUTTON_CR] = 1;	//押された
    				if(controlMode > 0)controlMode--;
    			}else if(ps3.read(BUTTON_CR) > 90 && swStat[BUTTON_CR] == 1){
    				swStat[BUTTON_CR] = 0;	//離された
    			}
        		//serial.printf("\n\r");
        		//serial.printf("%d,%d,%d\n\r",ps3.read(5),ps3.read(4),ps3.read(7));
        	}
    	}else{
    		if(rx[2].isReceive()){
    			//旋回方向は合ってる
    			//serial.printf("rv,");
        		mc.move((float)uchar4_to_int(rx[0].data)/10000 , (float)uchar4_to_int(rx[1].data)/10000 ,((float)uchar4_to_int(rx[2].data)/100000) + (+(float)(sa.read(2))/20000000));

        		motor[1].duty(mc.out[0]);
        		motor[2].duty(mc.out[1]);
        		motor[3].duty(mc.out[2]);
        		motor[0].duty(mc.out[3]);
    		}
    	}


    	for(int i=0;i<4;i++){
    		motor[i].cycle();
    	}

    	for(int i=0;i<6;i++){
    		led[i].cycle();
    	}

    	sw[0].cycle();
    	sw[1].cycle();
    	ps3.cycle();
    	sa.cycle();
    }
}
