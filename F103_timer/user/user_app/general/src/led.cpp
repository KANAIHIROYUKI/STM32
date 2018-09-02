#include "led.h"

void LED::setup(GPIO_TypeDef* gpioSet,uint16_t pinSet){
	gpio.setup(gpioSet,pinSet,OUTPUT);
	gpio.write(0);

	mode = LED_MODE_ON_OFF;

	intervalTimer = millis();

	System::cycleFunctionNumber++;
}

void LED::interval(uint32_t time){
	intervalT = time;
	intervalTimer = millis() + time;
	gpio.write(1);

	mode = LED_MODE_TOGGLE;
}

void LED::duty(uint32_t time,float duty){
	intervalT = 0;

	duty = floatlimit(0,duty,1.0);

	intervalON = time*duty;
	intervalOFF = time*(1.0-duty);

	dutyOFF = 0;
	dutyON  = millis() + intervalON;

	mode = LED_MODE_DUTY;
}

void LED::write(uint16_t stat){
	gpio.write(stat);
	mode = LED_MODE_ON_OFF;
}

void LED::cycle(){
	System::cycleFunctionCnt--;

	switch(mode){
	case LED_MODE_ON_OFF:

		break;

	case LED_MODE_TOGGLE:
		if(millis() > intervalTimer){
			intervalTimer += intervalT;
			gpio.toggle();
		}
		break;


	case LED_MODE_DUTY:
		if(millis() > dutyON){
			dutyOFF = dutyON + intervalOFF;
			gpio.write(0);
		}

		if(millis() > dutyOFF){
			dutyON  = dutyOFF + intervalON;
			gpio.write(1);
		}
		break;
	}
}
