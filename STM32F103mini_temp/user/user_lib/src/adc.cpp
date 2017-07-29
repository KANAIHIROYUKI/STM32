#include "adc.h"

uint8_t ADC::convertintgADC1Channel = 18;
uint8_t ADC::convertintgADC2Channel = 18;	//18でなし(16は内部温度計､17はVref)

void ADC::setup(ADC_TypeDef* adcSet,uint8_t channelSet,GPIO_TypeDef* gpio,uint16_t pin,uint8_t sampleTimeSet){
	ioSetup(gpio,pin);

	channel = channelSet;
	adc = adcSet;
	sampleTime = sampleTimeSet;
	if(adc == ADC1){
		ADC1Setup(channel);
	}else if(adc == ADC2){
		ADC2Setup(channel);
	}
}

int16_t ADC::read(){
	if(adc == ADC1){
		convertintgADC1Channel = channel;
		adcValueBuffer =  ADC1Read(channel,sampleTime);
		convertintgADC1Channel = 18;
		return adcValueBuffer;
	}else if(adc == ADC2){
		convertintgADC2Channel = channel;
		adcValueBuffer =  ADC2Read(channel,sampleTime);
		convertintgADC2Channel = 18;
		return adcValueBuffer;
	}else{
		return 0;
	}
}

int16_t ADC::read(uint8_t sampleTimeSet){
	if(adc == ADC1){
		convertintgADC1Channel = channel;
		adcValueBuffer =  ADC1Read(channel,sampleTime);
		convertintgADC1Channel = 18;
		return adcValueBuffer;
	}else if(adc == ADC2){
		convertintgADC2Channel = channel;
		adcValueBuffer =  ADC2Read(channel,sampleTime);
		convertintgADC2Channel = 18;
	}else{
		return 0;
	}
}

void ADC::start(){
	if(adc == ADC1){
		if(convertintgADC1Channel != channel && convertintgADC1Channel != 18)return;	//他のチャンネルが変換している時は邪魔しない

		if(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC) == SET){
			adcValueBuffer = ADC1Peek();
		}

		adcValueBuffer = 4096;	//AD変換中は4096
		convertintgADC1Channel = channel;
		ADC1Start(channel,sampleTime);

	}else if(adc == ADC2){
		if(convertintgADC2Channel != channel && convertintgADC2Channel != 18)return;

		if(ADC_GetFlagStatus(ADC2,ADC_FLAG_EOC) == SET){
			adcValueBuffer = ADC2Peek();
		}

		convertintgADC2Channel = channel;
		ADC2Start(channel,sampleTime);
	}
}

void ADC::start(uint8_t sampleTimeSet){
	adcValueBuffer = 4096;	//AD変換中は4096

	if(adc == ADC1){
		if(convertintgADC1Channel != channel && convertintgADC1Channel != 18)return;

		if(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC) == SET){
			adcValueBuffer = ADC1Peek();
		}

		convertintgADC1Channel = channel;
		ADC1Start(channel,sampleTimeSet);

	}else if(adc == ADC2){
		if(convertintgADC2Channel != channel && convertintgADC2Channel != 18)return;

		if(ADC_GetFlagStatus(ADC2,ADC_FLAG_EOC) == SET){
			adcValueBuffer = ADC2Peek();
		}

		convertintgADC2Channel = channel;
		ADC2Start(channel,sampleTimeSet);
	}
}

uint16_t ADC::peek(){

	if(adcValueBuffer != 4096){
		return adcValueBuffer;
	}

	adcValueBuffer = 0;
	if(adc == ADC1){
		adcValueBuffer = ADC1Peek();
		convertintgADC1Channel = 18;
	}else if(adc == ADC2){
		adcValueBuffer = ADC2Peek();
		convertintgADC2Channel = 18;
	}


	return adcValueBuffer;
}

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

	return  ADC_GetConversionValue(ADC1);
}

uint16_t ADC2Read(uint8_t ADC_Channel,uint8_t ADC_SampleTime){
	ADC_RegularChannelConfig(ADC2,ADC_Channel,1,ADC_SampleTime);
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
