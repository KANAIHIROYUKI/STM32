#include "GPIO.h"

void GPIO::setup(GPIO_TypeDef* gpio,uint16_t pin,GPIOMode_TypeDef mode){
	gpio_pin = pin;
	gpio_gpio = gpio;

	//if(gpio_gpio == GPIOA)return 0;

	RCC_APB2PeriphClockCmd(RCC_APB2ENR_IOPAEN,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2ENR_IOPBEN,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2ENR_IOPCEN,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2ENR_IOPDEN,ENABLE);

	gpio_mode = mode;

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = gpio_pin;
	GPIO_InitStructure.GPIO_Mode = gpio_mode;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(gpio_gpio,&GPIO_InitStructure);
}




uint16_t GPIO::write(BitAction value){
	/*if((gpio_mode != GPIO_Mode_Out_OD) || (gpio_mode != GPIO_Mode_Out_PP)){
		return 1;
	}*/
	GPIO_WriteBit(gpio_gpio,gpio_pin,value);

	return 0;
}

uint16_t GPIO::read(){
	return GPIO_ReadInputDataBit(gpio_gpio,gpio_pin);
}

void GPIO::toggle(){
	pinToggle(gpio_gpio,gpio_pin);
}

void GPIOSetup(){
	RCC_APB2PeriphClockCmd(RCC_APB2ENR_IOPAEN,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2ENR_IOPBEN,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2ENR_IOPCEN,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2ENR_IOPDEN,ENABLE);
}



void pinSetup(GPIO_TypeDef* gpio,uint16_t pin,GPIOMode_TypeDef mode){
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = pin;
	GPIO_InitStructure.GPIO_Mode = mode;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(gpio,&GPIO_InitStructure);
}

void pinWrite(GPIO_TypeDef* gpio,uint16_t pin,BitAction status){
	GPIO_WriteBit(gpio,pin,status);
}

void pinToggle(GPIO_TypeDef* gpio,uint16_t pin){
	if(GPIO_ReadOutputDataBit(gpio,pin) == Bit_SET){
		GPIO_WriteBit(gpio,pin,Bit_RESET);
	}else{
		GPIO_WriteBit(gpio,pin,Bit_SET);
	}
}

