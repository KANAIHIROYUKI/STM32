#include "main.hpp"

int main(void)
{
	setup();
	motorEn.write(1);

	led[0].interval(500);

	servoSet[0] = 0;	//シェイクロック
	servoSet[1] = 0;	//シェイカーロック
	servoSet[2] = 180;	//シェイカーグリップ
	servoSet[3] = 90;	//チルト

	servoSet[4] = 90;
	servoSet[5] = 0;
	servoSet[6] = 0;
	servoSet[7] = 0;

	servoDef[0] = 180;
	servoDef[1] = 170;
	servoDef[2] = 0;
	servoDef[3] = 0;

	servoDef[4] = 0;
	servoDef[5] = 0;
	servoDef[6] = 0;
	servoDef[7] = 0;

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
        		case 0://シェイク→ロック解除→持ち上げまで
        			motor[MOTOR_SHAKE].duty((float)hatTls(ps3.read(HAT_RY))/100);	//シェイカー
        			motor[MOTOR_TILT].duty((float)hatTls(ps3.read(HAT_LY))/100);	//巻取り

        			servoSwitchEvent(BUTTON_TR,SERVO_TILT_LOCK);
        			break;

        		case 1://ロック解除→シェイク機構退場

        			motor[MOTOR_SHAKE].duty((float)hatTls(ps3.read(HAT_RY))/100);	//シェイカー持ち上げ用
        			servoSwitchEvent(BUTTON_TR,SERVO_HOLDER_LOCK);
        			break;

        		case 2://シェイカー保持→注ぎ角度まで移動
        			servoSwitchEvent(BUTTON_TR,SERVO_GRIP);
        			motor[MOTOR_PICKUP].duty((float)hatTls(ps3.read(HAT_LY))/100);

        			ave.stack(hatTls(ps3.read(HAT_RY)));
        			if(ave.valueCnt > 10){
        				servoDeg[SERVO_POUT] -= ave.read()/5;
        				servoDeg[SERVO_POUT] = constrain(servoDeg[SERVO_POUT],0,180);
        				sa.write(SERVO_POUT+4,servoDeg[SERVO_POUT]);
            			ave.reset();
        			}
        			break;

        		case 3://注ぐ+押し出し
        			ave.stack(hatTls(ps3.read(HAT_RY)));
        			if(ave.valueCnt > 10){
        				servoDeg[SERVO_POUT] -= ave.read()/5;
        				servoDeg[SERVO_POUT] = constrain(servoDeg[SERVO_POUT],0,180);
        				sa.write(SERVO_POUT+4,servoDeg[SERVO_POUT]);
            			ave.reset();
        			}
        			motor[MOTOR_PUSH].duty((float)hatTls(ps3.read(HAT_LX))/100);	//シェイカー移動用


        			break;
        		case 4://予備
        			break;
        		case 5://自走
        			int_to_uchar4(tx[0].data,hatTls(ps3.read(HAT_RX)));	//x
            		int_to_uchar4(tx[1].data,hatTls(ps3.read(HAT_RY)));	//y
            		int_to_uchar4(tx[2].data,hatTls(ps3.read(HAT_LX)));	//rad

            		tx[0].send();
            		tx[1].send();
            		tx[2].send();
        			break;
        		default:
        			controlMode = 0;
        		}

        		/*****************************************************/

    			if(ps3.read(3) < 50 && swStat[3 + 8] == 0){			//モード選択
    				swStat[3 + 8] = 1;	//押された
    				controlMode++;
    			}else if(ps3.read(3) > 90 && swStat[3+8] == 1){
    				swStat[3 + 8] = 0;	//離された
    			}

    			if(ps3.read(2) < 50 && swStat[2 + 8] == 0){
    				swStat[2 + 8] = 1;	//押された
    				controlMode--;
    			}else if(ps3.read(2) > 90 && swStat[2+8] == 1){
    				swStat[2 + 8] = 0;	//離された
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
