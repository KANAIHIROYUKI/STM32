#include "gpio.h"

void GPIOSetup(){
	RCC_APB2PeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_AHBPeriph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_AHBPeriph_GPIOC,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_AHBPeriph_GPIOD,ENABLE);
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
