#include "gpio.h"

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

	gpio_invert = 0;
}




void GPIO::write(uint16_t value){
	/*if(gpio_invert){
		if(value){
			value = 0;
		}else{
			value = 1;
		}
	}*/

	if(value == 0){
		GPIO_WriteBit(gpio_gpio,gpio_pin,Bit_RESET);
	}else{
		GPIO_WriteBit(gpio_gpio,gpio_pin,Bit_SET);
	}
}

uint16_t GPIO::read(){
	return GPIO_ReadInputDataBit(gpio_gpio,gpio_pin);
}

void GPIO::toggle(){
	pinToggle(gpio_gpio,gpio_pin);
}

void GPIO::reset(){
	if(gpio_invert){
		write(1);
	}else{
		write(0);
	}
}

void GPIO::invert(uint16_t mode){
	gpio_invert = mode;
}


void GPIO::interruptSetup(EXTITrigger_TypeDef trigger){
	exti_trigger = trigger;
	EXTISetup(gpio_gpio,gpio_pin,exti_trigger);
}

void GPIO::interruptEnable(){
    EXTI_InitTypeDef EXTI_InitStructure;
    EXTI_StructInit(&EXTI_InitStructure);
    EXTI_InitStructure.EXTI_Line = gpio_pin;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = exti_trigger;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
}

void GPIO::interruptDesable(){
    EXTI_InitTypeDef EXTI_InitStructure;
    EXTI_StructInit(&EXTI_InitStructure);
    EXTI_InitStructure.EXTI_Line = gpio_pin;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = exti_trigger;
    EXTI_InitStructure.EXTI_LineCmd = DISABLE;
    EXTI_Init(&EXTI_InitStructure);
}



uint16_t GPIO::interruptFrag(){
	if(EXTI_GetITStatus(gpio_pin) == SET){
		EXTI_ClearITPendingBit(gpio_pin);
		return 1;
	}else{
		return 0;
	}
}

void GPIO::interruptFlagClear(){
	EXTI_ClearITPendingBit(gpio_pin);
}

/*********************************************************************************************************************/

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

void EXTISetup(GPIO_TypeDef* gpio,uint16_t pin,EXTITrigger_TypeDef trigger){
    EXTI_InitTypeDef EXTI_InitStructure;
    EXTI_StructInit(&EXTI_InitStructure);
    EXTI_InitStructure.EXTI_Line = pin;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = trigger;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

	for(uint32_t i=0;i<16;i++){
		if(pin == 1){
			pin = i;
			break;
		}
		pin = pin>>1;
	}

	if(gpio == GPIOA){
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,pin);
	}else if(gpio == GPIOB){
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,pin);
	}else if(gpio == GPIOC){
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,pin);
	}else if(gpio == GPIOD){
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOD,pin);
	}

    NVIC_InitTypeDef NVIC_InitStructure;
    switch(pin){
    case 0:
        NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
    	break;
    case 1:
        NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
    	break;
    case 2:
        NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
    	break;
    case 3:
        NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
    	break;
    case 4:
        NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
    	break;
    default:
    	if(pin < 10){
    	    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
    	}else{
    	    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    	}
    }
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
