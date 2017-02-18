#include "adc.h"

void ANALOG::setup(ADC_TypeDef* adc,uint8_t channel){
	adc_channel = channel;
	adc_adc = adc;
	if(adc_adc == ADC1){
		ADC1Setup(adc_channel);
	}
}

int16_t ANALOG::read(){
	if(adc_adc == ADC1){
		return ADC1Read(adc_channel);
	}else{
		return 0;
	}
}

void ANALOG::start(uint8_t ADC_SampleTime){
	if(adc_adc == ADC1){
		ADC1Start(adc_channel,ADC_SampleTime);
	}
}

uint16_t ANALOG::peek(){
	if(adc_adc == ADC1){
		return ADC1Peek();
	}else{
		return 0;
	}
}

void ADC1Setup(uint8_t ADC_Channel){
	RCC_ADCCLKConfig(RCC_CFGR_PLLSRC_HSI_Div2);
	RCC_APB2PeriphClockCmd(RCC_APB2ENR_ADC1EN,ENABLE);

	ADC_InitTypeDef ADC_InitStructure;
	ADC_StructInit(&ADC_InitStructure);

	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConvEdge_None;
	ADC_Init(ADC1,&ADC_InitStructure);

	ADC_ChannelConfig(ADC1,ADC_Channel,ADC_SampleTime_13_5Cycles);

	ADC_Cmd(ADC1,ENABLE);

	//ADC_ResetCalibration(ADC1);
	//while(ADC_GetResetCalibrationStatus(ADC1));
}

uint16_t ADC1Read(uint8_t ADC_Channel){
	ADC_ChannelConfig(ADC1,ADC_Channel,ADC_SampleTime_13_5Cycles);
	//ADC_StartOfConversion(ADC1);
	//while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC) == RESET);

	return  ADC_GetConversionValue(ADC1);
}

void ADC1Start(uint8_t ADC_Channel,uint8_t ADC_SampleTime){
	ADC_ChannelConfig(ADC1,ADC_Channel,ADC_SampleTime_13_5Cycles);
	ADC_StartOfConversion(ADC1);
}

uint16_t ADC1Peek(){
	return  ADC_GetConversionValue(ADC1);
}
