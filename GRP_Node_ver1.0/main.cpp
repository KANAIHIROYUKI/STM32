#include "main.hpp"

int main(void)
{
	setup();
	motorEn.write(1);

	led[0].interval(500);

	servoSet[0] = 180;	//シェイクロック
	servoSet[1] = 180;	//シェイカーロック
	servoSet[2] = 180;	//シェイカーグリップ
	servoSet[3] = 90;	//持ち上げ

	servoSet[4] = 90;	//チルト
	servoSet[5] = 0;
	servoSet[6] = 0;
	servoSet[7] = 0;

	for(int i=0;i<8;i++){
		sa.write(i + 4,0);
	}

    while(1){
    	if(millis() - intervalTime > 200){
    		intervalTime = millis();
    		//serial.printf("gy = %5d,",sa.read(2));
    		if(individual.read()){
    			serial.printf("mode = %d,",controlMode);
    			//serial.printf("%d,%d,",sa.read(0),sa.read(1));
        		for(int i=0;i<12;i++){
        			serial.printf("%d,",ps3.read(i));
        			//serial.printf("m%d = %d%%",i,(int)(mc.out[i]*100));
        		}
        		serial.printf("servo deg = ");
        		for(int i=0;i<8;i++){
        			serial.printf("%d,",servoDeg[i]);
        		}
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
        			motor[0].duty((float)ps3.read(0 + 4)/100);	//シェイカー
        			motor[1].duty((float)ps3.read(2 + 4)/100);	//巻取り

        			servoSwitchEvent(8+0,SERVO_TILT_LOCK);
        			break;

        		case 1://ロック解除→シェイク機構退場
        			motor[0].duty((float)ps3.read(0 + 4)/100);	//シェイカー持ち上げ用


        			servoSwitchEvent(8+0,SERVO_HOLDER_LOCK);
        			break;

        		case 2://シェイカー保持→注ぎ角度まで移動
        			servoSwitchEvent(8+0,SERVO_PICKUP);
        			servoSwitchEvent(8+1,SERVO_GRIP);
        			break;

        		case 3://注ぐ
        			servoSwitchEvent(8+0,SERVO_POUT);

        			break;
        		case 4://backup

        			break;
        		case 5://自走
            		int_to_uchar4(tx[0].data,ps3.read(7));
            		int_to_uchar4(tx[1].data,ps3.read(6));
            		int_to_uchar4(tx[2].data,ps3.read(5));
            		tx[0].send();
            		tx[1].send();
            		tx[2].send();
        			break;
        		default:
        			controlMode = 0;
        		}

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
