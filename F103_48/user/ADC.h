#ifndef ADC_H_
#define ADC_H_

#include "stm32f10x.h"
#include "stdio.h"

#define ADC_Cycle ADC_SampleTime_13Cycles5

class ADC{
public:
	void setup(ADC_TypeDef* adc,uint8_t channel);
	int16_t read();
private:
	ADC_TypeDef* adc_adc;
	uint8_t adc_channel;
};

void ADC1Setup(uint8_t);
uint16_t ADC1Read(uint8_t);

void ADC2Setup(uint8_t);
uint16_t ADC2Read(uint8_t);

#endif
