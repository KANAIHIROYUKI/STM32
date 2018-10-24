#include "main.hpp"

int main(void)
{
	setup();

	motorDuty[ServoP] = M_PI/2/3;		//90/3=30度
	motorDuty[ServoZ] = -M_PI/2 + M_PI/6;

	while(1){
		sys.cycle();
		limit.cycle();
		ps3.cycle();

		for(int i=0;i<3;i++){
			led[i].cycle();
		}

		if(intervalTime < millis()){
			intervalTime += 100;
			for(int i=0;i<10;i++){
				//serial.printf("p=%d,",(int)(motorDuty[i]*100));
			}
			serial.printf("prt = %d,",millis() - ps3.lastReceiveTime);
			serial.printf("p = %d,z = %d,",(int)(motorDuty[ServoP]*100),(int)(motorDuty[ServoZ]*100));
			serial.printf("%d,%d,%d",limit.read(),pot.read(),enc.read());
			serial.printf("%d,%d,%d,%d\n\r",ps3.data[PS3_HatRX],ps3.data[PS3_HatRY],ps3.data[PS3_HatLX],ps3.data[PS3_HatLY]);
		}

		if(ps3.update){
			ps3.update = 0;

			//伸縮
			if(ps3.data[PS3_R1]){
				motorDuty[MotorS] = -0.2;
			}else if(ps3.data[PS3_L1]){
				motorDuty[MotorS] = 0.2;
			}else{
				motorDuty[MotorS] = 0;
			}

			//足回り
			motorDuty[MoveX] = -ps3.data[PS3_HatLX];
			motorDuty[MoveY] = ps3.data[PS3_HatLY];
			motorDuty[MoveR] = ps3.data[PS3_HatRX];

			/*
			int_to_uchar4(tx[0].data,ps3.data[PS3_HatLX]);	//x
    		int_to_uchar4(tx[1].data,ps3.data[PS3_HatLY]);	//y
    		int_to_uchar4(tx[2].data,ps3.data[PS3_HatRX]);	//rad
    		*/

    		//ピッチ
    		motorDuty[MotorR] = (float)ps3.data[PS3_HatRY] / 256;

    		//ハンドサーボ
    		if(ps3.data[PS3_UP]){
    			motorDuty[ServoP] += 0.001;
    		}else if(ps3.data[PS3_DOWN]){
    			motorDuty[ServoP] -= 0.001;
    		}

    		if(ps3.data[PS3_RIGHT]){
    			motorDuty[ServoZ] += 0.001;
    		}else if(ps3.data[PS3_LEFT]){
    			motorDuty[ServoZ] -= 0.001;
    		}

    		//ハンド開閉
    		if(ps3.data[PS3_CIR]){
    			if(oldHand == 0){

        			if(handStat == 0){
        				handStat = 1;
        				vData[0] = 0xFF;
        				vData[1] = 0xFF;
        				valve.can->send(0x280,2,vData);
        			}else{
        				handStat = 0;
        				vData[0] = 0xFF;
        				vData[1] = 0x00;
        				valve.can->send(0x280,2,vData);
        			}
    			}

    			oldHand = 1;
    		}else{
    			oldHand = 0;
    		}



		}

		if(millis() - ps3.lastReceiveTime > 100){
			motorDuty[MotorR] = 0.01 + (0.015*pot.read()/800);
			for(int i=6;i<10;i++){
				motorDuty[i] = 0;
			}
		}

		if(controlCycleTime < millis()){
			controlCycleTime += 10;
			controlCycle();
		}
	}
}



void controlCycle(){

	if(pot.read() >	880	&& motorDuty[MotorS] < 0)motorDuty[MotorS] = 0;
	if(pot.read() <	10		&& motorDuty[MotorS] > 0)motorDuty[MotorS] = 0;
	md[MotorS].duty(motorDuty[MotorS]);
	md[MotorS_S].duty(motorDuty[MotorS]);

	if(limit.read() == 1){
		motorDuty[MotorR] = 0;
	}
	md[MotorR].duty(motorDuty[MotorR] + 0.01 + (0.015*pot.read()/800));
	md[MotorR_S].duty(motorDuty[MotorR] + 0.01 + (0.015*pot.read()/800));

	if(motorDuty[ServoP] < -M_PI/2)motorDuty[ServoP] = -M_PI/2;
	if(motorDuty[ServoP] > M_PI/2)motorDuty[ServoP] = M_PI/2;
	if(motorDuty[ServoZ] < -M_PI/2)motorDuty[ServoZ] = -M_PI/2;
	if(motorDuty[ServoZ] > M_PI/2)motorDuty[ServoZ] = M_PI/2;


	servo[0].position(159 - (motorDuty[ServoP]/M_PI)*40);
	servo[1].position(159 - (motorDuty[ServoZ]/M_PI)*40);

	int_to_uchar4(tx[0].data,motorDuty[MoveX]);	//x
	int_to_uchar4(tx[1].data,motorDuty[MoveY]);	//y
	int_to_uchar4(tx[2].data,motorDuty[MoveR]);	//rad

	tx[0].send();
	tx[1].send();
	tx[2].send();




	//valve.write(0,handStat);
	//valve.write(1,handStat);
}
