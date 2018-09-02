#include "main.hpp"

int main(void)
{
	setup();

	pwm.duty(1000);
	dwell.write(0);

	while(1){
		sys.cycle();
		led[0].cycle();
		led[1].cycle();

		if(intervalTime < millis()){
			intervalTime += 100;
			gdNss.write(0);
			delayMicros(100);
			for(int i=1;i<12;i++){
				serial.printf("%d,",spi.transfer(i));
			}
			gdNss.write(0);
			serial.printf("\n\r");

			//ss.write(0);
			//serial.printf("%4d,\n\r",(int)(((spi.transfer(0) << 3) + (spi.transfer(0) >> 5)) & 0x3FF));
			//ss.write(1);
			//drv.readFault();
			//serial.printf("%d,%d,%d,%d,%d,%d\n\r",drv.data[0],drv.data[1],drv.data[2],drv.data[3],nFault.read(),pwGd.read());
		}

		switch(millis()%6){
		case 0:
			state[0].write(0);
			state[0].write(1);
			state[0].write(0);
			break;

		case 1:
			state[0].write(0);
			state[0].write(1);
			state[0].write(1);
			break;

		case 2:
			state[0].write(0);
			state[0].write(0);
			state[0].write(1);
			break;

		case 3:
			state[0].write(1);
			state[0].write(0);
			state[0].write(1);
			break;

		case 4:
			state[0].write(1);
			state[0].write(0);
			state[0].write(0);
			break;

		case 5:
			state[0].write(1);
			state[0].write(1);
			state[0].write(0);
			break;
		}
	}
}
