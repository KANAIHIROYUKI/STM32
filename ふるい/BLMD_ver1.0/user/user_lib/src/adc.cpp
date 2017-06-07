#include "adc.h"

void ADC::setup(ADC_TypeDef* adc,uint8_t channel,GPIO_TypeDef* gpio,uint16_t pin){
	ioSetup(gpio,pin);

	adc_channel = channel;
	adc_adc = adc;
	if(adc_adc == ADC1){
		ADC1Setup(adc_channel);
	}else if(adc_adc == ADC2){
		ADC2Setup(adc_channel);
	}
}

void ADC::itSetup(uint16_t ADC_InterruptMode){
	interruptMode = ADC_InterruptMode;
	if(adc_adc == ADC1){
		ADC1ITSetup(ADC_InterruptMode);
	}else if(adc_adc == ADC2){
		ADC2ITSetup(ADC_InterruptMode);
	}
}

void ADC::interrupt(){
	if(adc_adc == ADC1){
		ADC_ClearITPendingBit(ADC1,interruptMode);
	}else if(adc_adc == ADC2){
		ADC_ClearITPendingBit(ADC2,interruptMode);
	}
}

int16_t ADC::read(){
	if(adc_adc == ADC1){
		return ADC1Read(adc_channel);
	}else if(adc_adc == ADC2){
		return ADC2Read(adc_channel);
	}else{
		return 0;
	}
}

void ADC::start(uint8_t ADC_SampleTime){
	if(adc_adc == ADC1){
		ADC1Start(adc_channel,ADC_SampleTime);
	}else if(adc_adc == ADC2){
		ADC2Start(adc_channel,ADC_SampleTime);
	}
}

uint16_t ADC::peek(){
	if(adc_adc == ADC1){
		return ADC1Peek();
	}else if(adc_adc == ADC2){
		return ADC2Peek();
	}else{
		return 0;
	}
}

void ADC::ioSetup(GPIO_TypeDef* gpio,uint16_t pin){
	GPIO IN;
	IN.setup(gpio,pin,INPUT_AIN);
}

void ADC1Setup(uint8_t ADC_Channel){
	RCC_ADCCLKConfig(RCC_CFGR_PLLSRC_HSI_Div2);
	RCC_APB2PeriphClockCmd(RCC_APB2ENR_ADC1EN,ENABLE);

	ADC_InitTypeDef ADC_InitStructure;
	ADC_StructInit(&ADC_InitStructure);

	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_Init(ADC1,&ADC_InitStructure);

	ADC_RegularChannelConfig(ADC1,ADC_Channel,1,ADC_SampleTime_13Cycles5);

	ADC_Cmd(ADC1,ENABLE);

	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1));
}

void ADC2Setup(uint8_t ADC_Channel){
	RCC_ADCCLKConfig(RCC_CFGR_PLLSRC_HSI_Div2);
	RCC_APB2PeriphClockCmd(RCC_APB2ENR_ADC2EN,ENABLE);

	ADC_InitTypeDef ADC_InitStructure;
	ADC_StructInit(&ADC_InitStructure);

	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_Init(ADC2,&ADC_InitStructure);

	ADC_RegularChannelConfig(ADC2,ADC_Channel,1,ADC_SampleTime_13Cycles5);

	ADC_Cmd(ADC2,ENABLE);

	ADC_ResetCalibration(ADC2);
	while(ADC_GetResetCalibrationStatus(ADC2));
	ADC_StartCalibration(ADC2);
	while(ADC_GetCalibrationStatus(ADC2));
}

void ADC1ITSetup(uint16_t ADC_InterruptMode){
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	ADC_ITConfig(ADC1,ADC_InterruptMode,ENABLE);
}

void ADC2ITSetup(uint16_t ADC_InterruptMode){
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	ADC_ITConfig(ADC2,ADC_InterruptMode,ENABLE);
}

uint16_t ADC1Read(uint8_t ADC_Channel){
	ADC_RegularChannelConfig(ADC1,ADC_Channel,1,ADC_SampleTime_13Cycles5);
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
	while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC) == RESET);

	return  ADC_GetConversionValue(ADC1);
}

uint16_t ADC2Read(uint8_t ADC_Channel){
	ADC_RegularChannelConfig(ADC2,ADC_Channel,1,ADC_SampleTime_13Cycles5);
	ADC_SoftwareStartConvCmd(ADC2,ENABLE);
	while(ADC_GetFlagStatus(ADC2,ADC_FLAG_EOC) == RESET);

	return  ADC_GetConversionValue(ADC2);
}

void ADC1Start(uint8_t ADC_Channel,uint8_t ADC_SampleTime){
	ADC_RegularChannelConfig(ADC1,ADC_Channel,1,ADC_SampleTime);
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
}

void ADC2Start(uint8_t ADC_Channel,uint8_t ADC_SampleTime){
	ADC_RegularChannelConfig(ADC2,ADC_Channel,1,ADC_SampleTime);
	ADC_SoftwareStartConvCmd(ADC2,ENABLE);
}

uint16_t ADC1Peek(){
	return  ADC_GetConversionValue(ADC1);
}

uint16_t ADC2Peek(){
	return  ADC_GetConversionValue(ADC2);
}
