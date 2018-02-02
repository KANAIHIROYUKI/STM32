#include "main.hpp"

int main(void)
{
	setup();
	motorEn.write(1);

	led[0].interval(500);

    while(1){
    	if(millis() - intervalTime > 200){
    		intervalTime = millis();
    		//serial.printf("gy = %5d,",sa.read(2));
    		if(individual.read()){
        		for(int i=0;i<12;i++){
        			serial.printf("%d,",ps3.read(i));
        			//serial.printf("m%d = %d%%",i,(int)(mc.out[i]*100));
        		}
    		}else{
    			serial.printf("%d,",(int)can.lastReceiveTime);
        		for(int i=0;i<4;i++){
        			//serial.printf("%d,",ps3.read(i));
        			serial.printf("m%d = %d%%",i,(int)(mc.out[i]*100));
        		}
    		}

    		serial.printf("\n\r");
    		//serial.printf("x = %d,y = %d,rad = %d\n\r",uchar4_to_int(rx[0].data),uchar4_to_int(rx[1].data),uchar4_to_int(rx[2].data));
    	}

    	if(individual.read()){
        	if(ps3.update){
        		ps3.update = 0;
        		int_to_uchar4(tx[0].data,ps3.read(5));
        		int_to_uchar4(tx[1].data,ps3.read(4));
        		int_to_uchar4(tx[2].data,ps3.read(7));
        		tx[0].send();
        		tx[1].send();
        		tx[2].send();
        		for(int i=0;i<4;i++){
        			sa.write(4 + i,100 - ps3.read(i));
        			motor[i].duty((float)ps3.read(4+i)/100);
        		}
        		//serial.printf("%d,%d,%d\n\r",ps3.read(5),ps3.read(4),ps3.read(7));
        	}
    	}else{
    		if(rx[2].isReceive()){
    			//ù‰ñ•ûŒü‚Í‡‚Á‚Ä‚é
        		mc.move((float)uchar4_to_int(rx[0].data)/10000 , (float)uchar4_to_int(rx[1].data)/10000 ,((float)uchar4_to_int(rx[2].data)/100000) + (+(float)(sa.read(2))/20000000));
        		for(int i=0;i<4;i++){
        			motor[i].duty(mc.out[i]);
        		}
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
