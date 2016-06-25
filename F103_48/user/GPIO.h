#ifndef GPIO_H
#define GPIO_H

#include "stm32f10x.h"
#include "stdio.h"




class GPIO {
public:
	int16_t setup(GPIO_TypeDef* gpio,uint16_t pin,GPIOMode_TypeDef mode);
	uint16_t write(BitAction value);
	uint16_t read();

private:
	uint16_t gpio_pin;
	GPIO_TypeDef* gpio_gpio;
	GPIOMode_TypeDef gpio_mode;
};

void GPIOSetup();
void pinSetup(GPIO_TypeDef* gpio,uint16_t pin,GPIOMode_TypeDef mode);
void pinWrite(GPIO_TypeDef* gpio,uint16_t pin,BitAction status);

#endif
