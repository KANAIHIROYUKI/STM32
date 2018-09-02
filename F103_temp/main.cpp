#include "main.hpp"

int main(void)
{
	setup();

	while(1){
		sys.cycle();
		robo.cycle();

		for(int i=0;i<3;i++){
			led[i].cycle();
		}

		if(intervalTime < millis()){
			intervalTime += 100;
			//serial.printf("%d,%d,%d,",(int)(robo.readR()*180/M_PI),(int)(robo.readS()),(int)(robo.rFF*100));
			//serial.printf("pot,%d,%d,enc,%d,%d\n\r",robo.pot[0]->read(),robo.pot[1]->read(),(int)robo.enc[0]->read(),(int)robo.enc[1]->read());
			//serial.printf("pid ,%d,%d,",(int)(robo.pid[MotorX].outputF()*100),(int)(robo.pid[MotorR].output32()*100));
			//serial.printf("md %5d,%5d,%5d,%5d  ",robo.md[0]->outDuty16,robo.md[1]->outDuty16,robo.md[2]->outDuty16,robo.md[3]->outDuty16);
			//serial.printf("J = %4d,R = %4d,S = %4d,X = %4d\n\r",(int)(robo.readJ()),(int)(robo.readR() * 360 / (2 * M_PI)),(int)(robo.readS()),(int)(robo.readX()));
		}
	}

    while(1){
    	sys.cycle();
    	ps3.cycle();

    	limit[0].cycle();
    	limit[1].cycle();

    	led[0].cycle();
    	led[1].cycle();
    	led[2].cycle();

    	if(intervalTime < millis()){
    		intervalTime += 100;
    		serial.printf("out = %d,",md[1].outDuty16);
    		serial.printf("%4d,%4d,%4d,enc %4d,%4d,\n\r",pot[0].read(),pot[1].read(),pot[2].read(),enc[0].read(),enc[1].read());
    	}
    	if(serial.available()){
    		rxTime = millis() + 100;
    		switch(serial.read()){
    		case 'w':
    			motorDuty[0] = 0.2;
    			break;
    		case 's':
    			motorDuty[0] = -0.2;
    			break;
    		case 'a':
    			motorDuty[1] = 0.2;
    			break;
    		case 'd':
    			motorDuty[1] = -0.2;
    			break;
    		default:
    			motorDuty[0] = 0;
    			motorDuty[1] = 0;

    		}
    	}
    	if(rxTime < millis()){
    		motorDuty[0] = 0;
    		motorDuty[1] = 0;
    		motorDuty[2] = 0;
    		motorDuty[3] = 0;
    	}

    	if(canIntervalTime < millis()){
    		canIntervalTime += 20;
    		for(int i=0;i<4;i++){
    			md[i].duty(motorDuty[i]);
    		}
    		/*
    		md[0].duty(((float)pot[0].read()/2047) - 1.0);
    		md[1].duty(((float)pot[0].read()/2047) - 1.0);
    		md[2].duty(((float)pot[1].read()/2047) - 1.0);
    		md[3].duty(((float)pot[1].read()/2047) - 1.0);*/
    	}
    }
}
