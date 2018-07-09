#ifndef ADC_H_
#define ADC_H_

#include "stm32f10x.h"
#include <stdio.h>

#include "gpio.h"
#include "system.h"

#define ADC_SampleTime_Default ADC_SampleTime_13Cycles5	//デフォルト値､2.04us､25.2kΩ

#define ADC10_Setup ADC1,0,PA0
#define ADC11_Setup ADC1,1,PA1
#define ADC12_Setup ADC1,2,PA2
#define ADC13_Setup ADC1,3,PA3
#define ADC14_Setup ADC1,4,PA4
#define ADC15_Setup ADC1,5,PA5
#define ADC16_Setup ADC1,6,PA6
#define ADC17_Setup ADC1,7,PA7
#define ADC18_Setup ADC1,8,PB0
#define ADC19_Setup ADC1,9,PB1

#define ADC20_Setup ADC2,0,PA0
#define ADC21_Setup ADC2,1,PA1
#define ADC22_Setup ADC2,2,PA2
#define ADC23_Setup ADC2,3,PA3
#define ADC24_Setup ADC2,4,PA4
#define ADC25_Setup ADC2,5,PA5
#define ADC26_Setup ADC2,6,PA6
#define ADC27_Setup ADC2,7,PA7
#define ADC28_Setup ADC2,8,PB0
#define ADC29_Setup ADC2,9,PB1

#define PA0ADC1_Setup ADC1,0,PA0
#define PA1ADC1_Setup ADC1,1,PA1
#define PA2ADC1_Setup ADC1,2,PA2
#define PA3ADC1_Setup ADC1,3,PA3
#define PA4ADC1_Setup ADC1,4,PA4
#define PA5ADC1_Setup ADC1,5,PA5
#define PA6ADC1_Setup ADC1,6,PA6
#define PA7ADC1_Setup ADC1,7,PA7
#define PB0ADC1_Setup ADC1,8,PB0
#define PB1ADC1_Setup ADC1,9,PB1

#define PA0ADC2_Setup ADC2,0,PA0
#define PA1ADC2_Setup ADC2,1,PA1
#define PA2ADC2_Setup ADC2,2,PA2
#define PA3ADC2_Setup ADC2,3,PA3
#define PA4ADC2_Setup ADC2,4,PA4
#define PA5ADC2_Setup ADC2,5,PA5
#define PA6ADC2_Setup ADC2,6,PA6
#define PA7ADC2_Setup ADC2,7,PA7
#define PB0ADC2_Setup ADC2,8,PB0
#define PB1ADC2_Setup ADC2,9,PB1

class ADC{
public:
	void setup(ADC_TypeDef* adc
			,uint8_t channel
			,GPIO_TypeDef* gpio,uint16_t pin
			,uint8_t sampleTimeSet = ADC_SampleTime_Default);

	//AD値読む｡while使っているので変換待ちする｡
	int16_t read();
	int16_t read(uint8_t ADC_SampleTime);

	//AD変換はじめ　値読むのはpeek()
	void start();
	void start(uint8_t ADC_SampleTime);

	int16_t converted();

	//AD変換済みの値を取得する
	uint16_t peek();

	uint16_t adcValueBuffer;	//AD変換中は4096
	uint8_t channel,sampleTime,adcStarted;


private:
	void ioSetup(GPIO_TypeDef* gpio,uint16_t pin);
	ADC_TypeDef* adc;
};

void ADC1Setup(uint8_t ADC_Channel);
void ADC2Setup(uint8_t ADC_Channel);

uint16_t ADC1Read(uint8_t ADC_Channel,uint8_t ADC_SampleTime = ADC_SampleTime_13Cycles5);
uint16_t ADC2Read(uint8_t ADC_Channel,uint8_t ADC_SampleTime = ADC_SampleTime_13Cycles5);

void ADC1Start(uint8_t ADC_Channel,uint8_t ADC_SampleTime = ADC_SampleTime_13Cycles5);
void ADC2Start(uint8_t ADC_Channel,uint8_t ADC_SampleTime = ADC_SampleTime_13Cycles5);

uint16_t ADC1GetValue();
uint16_t ADC2GetValue();

#endif
