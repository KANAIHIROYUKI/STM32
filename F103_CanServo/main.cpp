#include "main.hpp"

int main(void)
{
	setup();

	while(1){
		sys.cycle();
		if(intervalTime < millis()){
			intervalTime += 100;
			serial.printf("%d\n\r",servo[0].outDuty);
		}
	}
}
