#ifndef GPIO_H_
#define GPIO_H_

#include "stm32f10x.h"
#include <stdio.h>

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
#define OUTPUT_OD GPIO_Mode_Out_OD
#define OUTPUT_AF GPIO_Mode_AF_PP
#define OUTPUT_AF_OD GPIO_Mode_AF_OD

#define INPUT GPIO_Mode_IN_FLOATING
#define INPUT_AIN GPIO_Mode_AIN
#define INPUT_PU GPIO_Mode_IPU
#define INPUT_PD GPIO_Mode_IPD

class GPIO/* :public DigitalIn,public DigitalOut*/{
public:
	//ピンやモードはマクロ使うと楽
	void setup(GPIO_TypeDef* gpio,uint16_t pin,GPIOMode_TypeDef mode);

	void write(uint16_t value);
	uint16_t read();
	void toggle();

	//初期状態に戻す(不要そう)
	void reset();

	//入出力を反転する､mode=1なら反転状態､mode=0で非反転
	void IOInvert(uint16_t mode = 1);

	//割込みの設定(有効にはならない)
	void interruptSetup(EXTITrigger_TypeDef trigger);

	//割込みの有効･向こう
	void interruptEnable();
	void interruptDesable();

	//割込みフラグを読む
	uint16_t interruptFrag();

	//割込みフラグを折る
	void interruptFlagClear();

	uint16_t invert;
	uint16_t pin;
	GPIO_TypeDef* gpio;
	GPIOMode_TypeDef mode;

	EXTITrigger_TypeDef exti_trigger;
};

void GPIOSetup();
void pinSetup(GPIO_TypeDef* gpio,uint16_t pin,GPIOMode_TypeDef mode);
void pinWrite(GPIO_TypeDef* gpio,uint16_t pin,BitAction status);

void pinToggle(GPIO_TypeDef* gpio,uint16_t pin);

void EXTISetup(GPIO_TypeDef* gpio,uint16_t pin,EXTITrigger_TypeDef trigger);

#endif
