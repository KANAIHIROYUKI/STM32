#ifndef ADC_H_
#define ADC_H_

#include "stm32f30x_conf.h"

#define ADC_Cycle ADC_SampleTime_19Cycles5

void ADC1Setup(uint8_t ADC_Channel);
void ADC2Setup(uint8_t ADC_Channel);
void ADC3Setup(uint8_t ADC_Channel);
void ADC4Setup(uint8_t ADC_Channel);


uint16_t ADC1Read(uint8_t ADC_Channel);
uint16_t ADC2Read(uint8_t ADC_Channel);
uint16_t ADC3Read(uint8_t ADC_Channel);
uint16_t ADC4Read(uint8_t ADC_Channel);

void ADC1Start(uint8_t ADC_Channel,uint8_t ADC_SampleTime = ADC_Cycle);
void ADC2Start(uint8_t ADC_Channel,uint8_t ADC_SampleTime = ADC_Cycle);
void ADC3Start(uint8_t ADC_Channel,uint8_t ADC_SampleTime = ADC_Cycle);
void ADC4Start(uint8_t ADC_Channel,uint8_t ADC_SampleTime = ADC_Cycle);

uint16_t ADC1Peek();
uint16_t ADC2Peek();
uint16_t ADC3Peek();
uint16_t ADC4Peek();

#endif
