#ifndef ADC_H_
#define ADC_H_

#include "stm32f10x.h"
#include "stdio.h"

#define ADC_Cycle ADC_SampleTime_13Cycles5

class ADC{
public:
	void setup(ADC_TypeDef* adc,uint8_t channel);
	int16_t read();
	void start(uint8_t ADC_SampleTime = ADC_SampleTime_13Cycles5);
	uint16_t peek();
private:
	ADC_TypeDef* adc_adc;
	uint8_t adc_channel;
};

void ADC1Setup(uint8_t ADC_Channel);
void ADC2Setup(uint8_t ADC_Channel);

uint16_t ADC1Read(uint8_t ADC_Channel);
uint16_t ADC2Read(uint8_t ADC_Channel);

void ADC1Start(uint8_t ADC_Channel,uint8_t ADC_SampleTime = ADC_SampleTime_13Cycles5);
void ADC2Start(uint8_t ADC_Channel,uint8_t ADC_SampleTime = ADC_SampleTime_13Cycles5);

uint16_t ADC1Peek();
uint16_t ADC2Peek();

#endif
