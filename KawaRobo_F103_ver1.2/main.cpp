#include "main.hpp"

int main(void)
{
	setup();
	motorEn.write(1);

    while(1){

    	//mag.cycle();
    	/*if(millis() - intervalTime > 200){
    		intervalTime = millis();
    		//spiNss.write(0);
    		//serial.printf("%6d,\n\r",((spi.transfer(0)<<8) + (spi.transfer(0)))>>5);
    		//spiNss.write(1);
    		//serial.printf("0x%4x,mag cnt = %6d,rx = %3d,tx = %3d\n\r",mag.directlyValue[0],mag.valueReadCnt,spi.spi2rxBuffer.length(),spi.spi2txBuffer.length());

    		/*serial.printf("%d,\n\r",((spi.read() << 8) + spi.read()) >> 5);
    		spiNss.write(0);
    		spi.write(0xAA);
    		spi.write(0x55);
    		spi.send();
    		delayMicros(200);
    		spiNss.write(1);//

    		for(int i=0;i<16;i++){
    			serial.printf("%4d,",ps3.read(i));
			}
			serial.printf("\n\r");

    	}//*/
    	/*
		if(ps3.update){
			ps3.update = 0;
			for(int i=0;i<4;i++){
				motor[i].duty((float)ps3.read(i+4)/100);
			}
		}*/

    	sys.cycle();
    	//ps3.cycle();
    	kw.cycle();
    }
}
