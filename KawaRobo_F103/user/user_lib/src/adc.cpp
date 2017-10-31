#include "adc.h"


void ADC::setup(ADC_TypeDef* adcSet,uint8_t channelSet,GPIO_TypeDef* gpio,uint16_t pin,uint8_t sampleTimeSet){
	ioSetup(gpio,pin);

	if(adcSet != ADC1 && adcSet != ADC2)System::error++;	//エラー追加

	channel = channelSet;
	adc = adcSet;
	sampleTime = sampleTimeSet;
	if(adc == ADC1){
		ADC1Setup(channel);
	}else if(adc == ADC2){
		ADC2Setup(channel);
	}

	adcStarted = 0;
}

int16_t ADC::read(){
	return read(sampleTime);
}

int16_t ADC::read(uint8_t sampleTimeSet){
	start(sampleTimeSet);
	while(converted() == 0);
	return peek();
}

void ADC::start(){
	start(sampleTime);
}

void ADC::start(uint8_t sampleTimeSet){

	if(adc == ADC1){
		if(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC) == SET)adcValueBuffer = ADC1GetValue();		//他のチャンネルのデータである可能性がある｡
		ADC1Start(channel,sampleTimeSet);													//他のチャンネルが変換中だった場合にどうなるのか？

	}else if(adc == ADC2){
		if(ADC_GetFlagStatus(ADC2,ADC_FLAG_EOC) == SET)adcValueBuffer = ADC2GetValue();
		ADC2Start(channel,sampleTimeSet);
	}
}

int16_t ADC::converted(){
	if(ADC_GetFlagStatus(adc,ADC_FLAG_EOC) == SET){
		return 1;
	}
	return 0;
}

uint16_t ADC::peek(){

	if(adc == ADC1){
		if(converted())adcValueBuffer = ADC1GetValue();

	}else if(adc == ADC2){
		if(converted())adcValueBuffer = ADC2GetValue();

	}else{

		adcValueBuffer = 0;
	}

	return adcValueBuffer;
}

/********************************************************************************************/

void ADC::ioSetup(GPIO_TypeDef* gpio,uint16_t pin){
	GPIO IN;
	IN.setup(gpio,pin,INPUT_AIN);
}

/*******************************↑C++/C↓*****************************************************/

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


// ADC Setup End

uint16_t ADC1Read(uint8_t ADC_Channel,uint8_t ADC_SampleTime){
	ADC_RegularChannelConfig(ADC1,ADC_Channel,1,ADC_SampleTime);
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
	while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC) == RESET);

	return  ADC1GetValue();
}

uint16_t ADC2Read(uint8_t ADC_Channel,uint8_t ADC_SampleTime){
	ADC_RegularChannelConfig(ADC2,ADC_Channel,1,ADC_SampleTime);
	ADC_SoftwareStartConvCmd(ADC2,ENABLE);
	while(ADC_GetFlagStatus(ADC2,ADC_FLAG_EOC) == RESET);

	return  ADC2GetValue();
}

void ADC1Start(uint8_t ADC_Channel,uint8_t ADC_SampleTime){
	ADC_RegularChannelConfig(ADC1,ADC_Channel,1,ADC_SampleTime);
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
}

void ADC2Start(uint8_t ADC_Channel,uint8_t ADC_SampleTime){
	ADC_RegularChannelConfig(ADC2,ADC_Channel,1,ADC_SampleTime);
	ADC_SoftwareStartConvCmd(ADC2,ENABLE);
}


uint16_t ADC1GetValue(){
	return  ADC_GetConversionValue(ADC1);
}

uint16_t ADC2GetValue(){
	return  ADC_GetConversionValue(ADC2);
}

/********************************************************************/
