#include "main.hpp"

int main(void)
{
	setup();
	//motorEn.write(1);

    while(1){
    	sys.cycle();
    	if(intervalTime < millis()){
    		intervalTime += 50;
    		//serial.printf("%d,\n\r",adc.read()*4);
    		pwm.duty(adc.read()*4);
    	}
    	if(serial.available()){
    		serial.printf("%d\n\r",serial.read());
    	}
    }
}
