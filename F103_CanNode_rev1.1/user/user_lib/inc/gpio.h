#ifndef GPIO_H_
#define GPIO_H_

#include "stm32f10x.h"
#include "stdio.h"

#define PA0 GPIOA,GPIO_Pin_0
#define PA1 GPIOA,GPIO_Pin_1
#define PA2 GPIOA,GPIO_Pin_2
#define PA3 GPIOA,GPIO_Pin_3
#define PA4 GPIOA,GPIO_Pin_4
#define PA5 GPIOA,GPIO_Pin_5
#define PA6 GPIOA,GPIO_Pin_6
#define PA7 GPIOA,GPIO_Pin_7

#define PA8 GPIOA,GPIO_Pin_8
#define PA9 GPIOA,GPIO_Pin_9
#define PA10 GPIOA,GPIO_Pin_10
#define PA11 GPIOA,GPIO_Pin_11
#define PA12 GPIOA,GPIO_Pin_12
#define PA13 GPIOA,GPIO_Pin_13
#define PA14 GPIOA,GPIO_Pin_14
#define PA15 GPIOA,GPIO_Pin_15

#define PB0 GPIOB,GPIO_Pin_0
#define PB1 GPIOB,GPIO_Pin_1
#define PB2 GPIOB,GPIO_Pin_2
#define PB3 GPIOB,GPIO_Pin_3
#define PB4 GPIOB,GPIO_Pin_4
#define PB5 GPIOB,GPIO_Pin_5
#define PB6 GPIOB,GPIO_Pin_6
#define PB7 GPIOB,GPIO_Pin_7

#define PB8 GPIOB,GPIO_Pin_8
#define PB9 GPIOB,GPIO_Pin_9
#define PB10 GPIOB,GPIO_Pin_10
#define PB11 GPIOB,GPIO_Pin_11
#define PB12 GPIOB,GPIO_Pin_12
#define PB13 GPIOB,GPIO_Pin_13
#define PB14 GPIOB,GPIO_Pin_14
#define PB15 GPIOB,GPIO_Pin_15

#define PC13 GPIOC,GPIO_Pin_13
#define PC14 GPIOC,GPIO_Pin_14
#define PC15 GPIOC,GPIO_Pin_15

#define PD0 GPIOD,GPIO_Pin_0
#define PD1 GPIOD,GPIO_Pin_1
#define PD2 GPIOD,GPIO_Pin_2
#define PD3 GPIOD,GPIO_Pin_3

#define OUTPUT GPIO_Mode_Out_PP
#define OUTPUT_UD GPIO_Mode_Out_OD
#define OUTPUT_AF GPIO_Mode_AF_PP
#define OUTPUT_AF_OD GPIO_Mode_AF_OD

#define INPUT GPIO_Mode_IN_FLOATING
#define INPUT_AIN GPIO_Mode_AIN
#define INPUT_PU GPIO_Mode_IPU
#define INPUT_PD GPIO_Mode_IPD

class GPIO {
public:
	void setup(GPIO_TypeDef* gpio,uint16_t pin,GPIOMode_TypeDef mode);
	uint16_t write(BitAction value);
	uint16_t read();
	void toggle();

//private:
	uint16_t gpio_pin;
	GPIO_TypeDef* gpio_gpio;
	GPIOMode_TypeDef gpio_mode;
};

void GPIOSetup();
void pinSetup(GPIO_TypeDef* gpio,uint16_t pin,GPIOMode_TypeDef mode);
void pinWrite(GPIO_TypeDef* gpio,uint16_t pin,BitAction status);

void pinToggle(GPIO_TypeDef* gpio,uint16_t pin);

#endif
