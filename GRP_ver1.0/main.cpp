#include "main.hpp"

int main(void)
{
	setup();
	motorEn.write(1);

	led[0].interval(500);

	delay(100);
	for(int i=0;i<10;i++){
		SPI_Master::spi2txBuffer.write(i);
	}
	for(int i=0;i<10;i++){

		serial.printf("data = %d,len = %d,emp = %d\n\r",SPI_Master::spi2txBuffer.read(),SPI_Master::spi2txBuffer.length(),SPI_Master::spi2txBuffer.isEmpty());
	}
	serial.printf("end");

    while(1){
    	if(millis() - intervalTime > 200){
    		intervalTime = millis();
    		//spiNss.write(0);
    		//serial.printf("%6d,\n\r",((spi.transfer(0)<<8) + (spi.transfer(0)))>>5);
    		//spiNss.write(1);
    		//serial.printf("0x%4x,mag cnt = %6d,rx = %3d,tx = %3d\n\r",mag.directlyValue[0],mag.valueReadCnt,spi.spi2rxBuffer.length(),spi.spi2txBuffer.length());

    		spiNss.write(0);
    		serial.printf("buf = %d,data = %d,",spi.available(),(spi.transfer() << 8 + spi.transfer())>>5);
    		//while(spi.available())spi.read();
    		//serial.printf("cnt = %d,l = ",SPI_Master::interruptCnt);
    		//serial.printf("%d,",SPI_Master::spi2txBuffer.length());

    		//spi.write(0xC3);
    		//spi.write(0xAA);
    		//spi.write(0);
    		//spi.write(0);

    		//spi.write(0);
    		//spi.write(0);
    		//spi.send();
    		delayMicros(200);
    		spiNss.write(1);//*/


    		//serial.printf("%d,",mag.read());
    		serial.printf("\n\r");
    	}//*/

    	/*
		if(ps3.update){
			ps3.update = 0;
			mc.move((float)ps3.read(7)/100,(float)ps3.read(6)/100,(float)ps3.read(5)/800);
			if(ps3.read(0) < 0){
				for(int i=0;i<4;i++){
					motor[i].duty(mc.out[i]/500);
				}
			}else{
				for(int i=0;i<4;i++){
					motor[i].duty(0);
				}
			}

		}

		if(sw[0].readStat && sw[0].read() == 0){
			sw[0].readStat = 0;
			serial.printf("&");
			//ps3.write(0,1);
		}*/

    	led[0].cycle();
    	mag.cycle();
    }
}
