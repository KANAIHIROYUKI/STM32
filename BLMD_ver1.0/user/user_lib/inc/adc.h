#ifndef ADC_H_
#define ADC_H_

#include "system.h"

#define ADC_Cycle ADC_SampleTime_28Cycles5	//2.04us¤25.2kƒ¶

class ADC{
public:
	void setup(ADC_TypeDef* adc,uint8_t channel,GPIO_TypeDef* gpio,uint16_t pin);
	void itSetup(uint16_t ADC_InterruptMode = ADC_IT_EOC);
	void interrupt();
	int16_t read();
	void start(uint8_t ADC_SampleTime = ADC_SampleTime_13Cycles5);
	uint16_t peek();
private:
	void ioSetup(GPIO_TypeDef* gpio,uint16_t pin);

	ADC_TypeDef* adc_adc;
	uint8_t adc_channel;
	uint16_t interruptMode;
};

void ADC1Setup(uint8_t ADC_Channel);
void ADC2Setup(uint8_t ADC_Channel);

void ADC1ITSetup(uint16_t ADC_InterruptMode = ADC_IT_EOC);
void ADC2ITSetup(uint16_t ADC_InterruptMode = ADC_IT_EOC);

uint16_t ADC1Read(uint8_t ADC_Channel);
uint16_t ADC2Read(uint8_t ADC_Channel);

void ADC1Start(uint8_t ADC_Channel,uint8_t ADC_SampleTime = ADC_SampleTime_13Cycles5);
void ADC2Start(uint8_t ADC_Channel,uint8_t ADC_SampleTime = ADC_SampleTime_13Cycles5);

uint16_t ADC1Peek();
uint16_t ADC2Peek();

#endif
