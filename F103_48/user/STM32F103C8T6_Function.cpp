#include "STM32F103C8T6_Function.h"


void GPIO::setup(GPIO_TypeDef* gpio,uint16_t pin,GPIOMode_TypeDef mode){
	gpio_pin = pin;
	gpio_gpio = gpio;

	RCC_APB2PeriphClockCmd(RCC_APB2ENR_IOPAEN,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2ENR_IOPBEN,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2ENR_IOPCEN,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2ENR_IOPDEN,ENABLE);
	gpio_mode = mode;

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = gpio_pin;
	GPIO_InitStructure.GPIO_Mode = gpio_mode;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(gpio_gpio,&GPIO_InitStructure);
}

uint16_t GPIO::write(BitAction value){
	/*if((gpio_mode != GPIO_Mode_Out_OD) || (gpio_mode != GPIO_Mode_Out_PP)){
		return 1;
	}*/
	GPIO_WriteBit(gpio_gpio,gpio_pin,value);

	return 0;
}

uint16_t GPIO::read(){
	int16_t value = 0;


	return value;
}

void GPIOSetup(){
	RCC_APB2PeriphClockCmd(RCC_APB2ENR_IOPAEN,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2ENR_IOPBEN,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2ENR_IOPCEN,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2ENR_IOPDEN,ENABLE);
}



void pinSetup(GPIO_TypeDef* gpio,uint16_t pin,GPIOMode_TypeDef mode){
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = pin;
	GPIO_InitStructure.GPIO_Mode = mode;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(gpio,&GPIO_InitStructure);
}

void pinWrite(GPIO_TypeDef* gpio,uint16_t pin,BitAction status){
	GPIO_WriteBit(gpio,pin,status);
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

void TIM2PWMSetup(uint16_t count){
	RCC_APB1PeriphClockCmd(RCC_APB1ENR_TIM2EN,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2ENR_AFIOEN,ENABLE);

	TIM_TimeBaseInitTypeDef TimeBaseInitStructure;
	TIM_TimeBaseStructInit(&TimeBaseInitStructure);
	TimeBaseInitStructure.TIM_Period = count;
	TIM_TimeBaseInit(TIM2,&TimeBaseInitStructure);
}

void TIM3PWMSetup(uint16_t count){
	RCC_APB1PeriphClockCmd(RCC_APB1ENR_TIM3EN,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2ENR_AFIOEN,ENABLE);

	TIM_TimeBaseInitTypeDef TimeBaseInitStructure;
	TIM_TimeBaseStructInit(&TimeBaseInitStructure);
	TimeBaseInitStructure.TIM_Period = count;
	TIM_TimeBaseInit(TIM3,&TimeBaseInitStructure);
}

void TIM4PWMSetup(uint16_t count){
	RCC_APB1PeriphClockCmd(RCC_APB1ENR_TIM4EN,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2ENR_AFIOEN,ENABLE);

	TIM_TimeBaseInitTypeDef TimeBaseInitStructure;
	TIM_TimeBaseStructInit(&TimeBaseInitStructure);
	TimeBaseInitStructure.TIM_Period = count;
	TIM_TimeBaseInit(TIM4,&TimeBaseInitStructure);
}



void OC1PWMSetup(TIM_TypeDef *tim,uint16_t mode){
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = mode;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;//duty
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(tim,&TIM_OCInitStructure);
}

void OC2PWMSetup(TIM_TypeDef *tim,uint16_t mode){
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = mode;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;//duty
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC2Init(tim,&TIM_OCInitStructure);

	TIM_ARRPreloadConfig(TIM3,ENABLE);
	TIM_Cmd(TIM3,ENABLE);
}

void OC3PWMSetup(TIM_TypeDef *tim,uint16_t mode){
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = mode;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;//duty
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC3Init(tim,&TIM_OCInitStructure);

	TIM_ARRPreloadConfig(TIM3,ENABLE);
	TIM_Cmd(TIM3,ENABLE);
}

void OC4PWMSetup(TIM_TypeDef *tim,uint16_t mode){
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = mode;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;//duty
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC4Init(tim,&TIM_OCInitStructure);

	TIM_ARRPreloadConfig(TIM3,ENABLE);
	TIM_Cmd(TIM3,ENABLE);
}

void OC1DutySet(TIM_TypeDef*TIMx,uint16_t duty){
	TIMx->CCR1 = duty;
}

void OC2DutySet(TIM_TypeDef*TIMx,uint16_t duty){
	TIMx->CCR2 = duty;
}

void OC3DutySet(TIM_TypeDef*TIMx,uint16_t duty){
	TIMx->CCR3 = duty;
}

void OC4DutySet(TIM_TypeDef*TIMx,uint16_t duty){
	TIMx->CCR4 = duty;
}

void USART2Setup(uint32_t baud){
	RCC_APB1PeriphClockCmd(RCC_APB1ENR_USART2EN,ENABLE);

	USART_InitTypeDef USARTInitStructure;
	USART_StructInit(&USARTInitStructure);
	USARTInitStructure.USART_BaudRate = baud;
	USARTInitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USARTInitStructure.USART_Parity = USART_Parity_No;
	USARTInitStructure.USART_StopBits = USART_CR2_STOP_1;
	USARTInitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART2,&USARTInitStructure);

	USART_Cmd(USART2,ENABLE);
}

void USART1Setup(uint32_t baud){
	RCC_APB2PeriphClockCmd(RCC_APB2ENR_USART1EN,ENABLE);

	USART_InitTypeDef USARTInitStructure;
	USART_StructInit(&USARTInitStructure);
	USARTInitStructure.USART_BaudRate = baud;
	USARTInitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USARTInitStructure.USART_Parity = USART_Parity_No;
	USARTInitStructure.USART_StopBits = USART_CR2_STOP_1;
	USARTInitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1,&USARTInitStructure);

	USART_Cmd(USART1,ENABLE);
}


void USART2ITSetup(uint16_t flag){
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_ITConfig(USART2,flag,ENABLE);
}

void USART1ITSetup(uint16_t flag){
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_ITConfig(USART1,flag,ENABLE);
}

void CAN1Setup(){
	CAN_DeInit(CAN1);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1,ENABLE);

	CAN_InitTypeDef CAN_InitStructure;
	CAN_StructInit(&CAN_InitStructure);
	CAN_InitStructure.CAN_TTCM = DISABLE;
	CAN_InitStructure.CAN_ABOM = DISABLE;
	CAN_InitStructure.CAN_AWUM = DISABLE;
	CAN_InitStructure.CAN_NART = DISABLE;
	CAN_InitStructure.CAN_TXFP = DISABLE;
	CAN_InitStructure.CAN_Mode = CAN_Mode_LoopBack;
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
	CAN_InitStructure.CAN_BS1 = CAN_BS1_6tq;
	CAN_InitStructure.CAN_BS2 = CAN_BS2_7tq;
	CAN_InitStructure.CAN_Prescaler = 4;
	CAN_Init(CAN1,&CAN_InitStructure);

	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
    CAN_FilterInitStructure.CAN_FilterNumber = 0;
    CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
    CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
    CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
    CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
    CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
    CAN_FilterInit(&CAN_FilterInitStructure);

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);
}

void CAN1Send(uint16_t id,uint8_t length,uint8_t *data){
    CanTxMsg TxMessage;
    TxMessage.StdId = id;
    TxMessage.IDE   = CAN_ID_STD;
    TxMessage.RTR   = CAN_RTR_DATA;
    TxMessage.DLC   = length;
    TxMessage.Data[0] = data[0];
    TxMessage.Data[1] = data[1];
    TxMessage.Data[2] = data[2];
    TxMessage.Data[3] = data[3];
    TxMessage.Data[4] = data[4];
    TxMessage.Data[5] = data[5];
    TxMessage.Data[6] = data[6];
    TxMessage.Data[7] = data[7];

    CAN_Transmit(CAN1,&TxMessage);
}


extern "C" void SysTick_Handler(void){
	if(TimingDelay != 0){
		TimingDelay--;
	}
}

void delay(uint32_t nTime){
	TimingDelay = nTime;
	while(TimingDelay != 0);
}



