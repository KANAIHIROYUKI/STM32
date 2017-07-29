#ifndef ADC_H_
#define ADC_H_

#include "system.h"

#define ADC_SampleTime_Default ADC_SampleTime_13Cycles5	//デフォルト値､2.04us､25.2kΩ

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

	//AD変換済みの値を取得する
	uint16_t peek();

	uint16_t adcValueBuffer;	//AD変換中は4096
	uint8_t channel,sampleTime;

	static uint8_t convertintgADC1Channel,convertintgADC2Channel;

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

uint16_t ADC1Peek();
uint16_t ADC2Peek();

#endif
