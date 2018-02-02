#ifndef _LED_H_
#define _LED_H_

#include "system.h"
#include "util.h"

#define LED_MODE_ON_OFF 0
#define LED_MODE_TOGGLE 1
#define LED_MODE_DUTY   2

class LED{
public:
	void setup(GPIO_TypeDef* gpio,uint16_t pin);
	void setup(GPIO *gpio);

	void interval(uint32_t time);
	void duty(uint32_t time,float duty);
	void write(uint16_t state);

	void cycle();

	uint16_t mode;
	uint32_t intervalON,intervalOFF,intervalT;
	uint64_t intervalTimer,dutyON,dutyOFF;
	GPIO gpio;
};








#endif
