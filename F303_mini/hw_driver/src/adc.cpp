#include "adc.h"

void ADC1Setup(uint8_t ADC_Channel){
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_ADC12,ENABLE);

	ADC_InitTypeDef ADC_InitStructure;
	ADC_StructInit(&ADC_InitStructure);

	ADC_Init(ADC1,&ADC_InitStructure);

	ADC_RegularChannelConfig(ADC1,ADC_Channel,1,ADC_Cycle);

	ADC_Cmd(ADC1,ENABLE);

	//ADC_ResetCalibration(ADC1);
	//while(ADC_GetResetCalibrationStatus(ADC1));
	//ADC_StartCalibration(ADC1);
	//while(ADC_GetCalibrationStatus(ADC1));
}

void ADC2Setup(uint8_t ADC_Channel){
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_ADC12,ENABLE);

	ADC_InitTypeDef ADC_InitStructure;
	ADC_StructInit(&ADC_InitStructure);

	ADC_Init(ADC2,&ADC_InitStructure);

	ADC_RegularChannelConfig(ADC2,ADC_Channel,1,ADC_Cycle);

	ADC_Cmd(ADC2,ENABLE);

	//ADC_ResetCalibration(ADC2);
	//while(ADC_GetResetCalibrationStatus(ADC2));
	ADC_StartCalibration(ADC2);
	while(ADC_GetCalibrationStatus(ADC2));
}

void ADC3Setup(uint8_t ADC_Channel){
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_ADC34,ENABLE);

	ADC_InitTypeDef ADC_InitStructure;
	ADC_StructInit(&ADC_InitStructure);

	ADC_Init(ADC3,&ADC_InitStructure);

	ADC_RegularChannelConfig(ADC3,ADC_Channel,1,ADC_Cycle);

	ADC_Cmd(ADC3,ENABLE);

	//ADC_ResetCalibration(ADC3);
	//while(ADC_GetResetCalibrationStatus(ADC3));
	ADC_StartCalibration(ADC3);
	while(ADC_GetCalibrationStatus(ADC3));
}

void ADC4Setup(uint8_t ADC_Channel){
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_ADC34,ENABLE);

	ADC_InitTypeDef ADC_InitStructure;
	ADC_StructInit(&ADC_InitStructure);

	ADC_Init(ADC4,&ADC_InitStructure);

	ADC_RegularChannelConfig(ADC4,ADC_Channel,1,ADC_Cycle);

	ADC_Cmd(ADC4,ENABLE);

	//ADC_ResetCalibration(ADC4);
	//while(ADC_GetResetCalibrationStatus(ADC4));
	ADC_StartCalibration(ADC4);
	while(ADC_GetCalibrationStatus(ADC4));
}

uint16_t ADC1Read(uint8_t ADC_Channel){
	ADC_RegularChannelConfig(ADC1,ADC_Channel,1,ADC_Cycle);
	ADC_StartConversion(ADC1);
	while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC) == RESET);

	return  ADC_GetConversionValue(ADC1);
}

uint16_t ADC2Read(uint8_t ADC_Channel){
	ADC_RegularChannelConfig(ADC2,ADC_Channel,1,ADC_Cycle);
	ADC_StartConversion(ADC2);
	while(ADC_GetFlagStatus(ADC2,ADC_FLAG_EOC) == RESET);

	return  ADC_GetConversionValue(ADC2);
}

void ADC1Start(uint8_t ADC_Channel,uint8_t ADC_SampleTime){
	ADC_RegularChannelConfig(ADC1,ADC_Channel,1,ADC_SampleTime);
	ADC_StartConversion(ADC1);
}

void ADC2Start(uint8_t ADC_Channel,uint8_t ADC_SampleTime){
	ADC_RegularChannelConfig(ADC2,ADC_Channel,1,ADC_SampleTime);
	ADC_StartConversion(ADC2);
}

uint16_t ADC1Peek(){
	return  ADC_GetConversionValue(ADC1);
}

uint16_t ADC2Peek(){
	return  ADC_GetConversionValue(ADC2);
}
