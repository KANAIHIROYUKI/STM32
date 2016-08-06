#include "TIM.h"


uint16_t PWM::setup(TIM_TypeDef* tim,uint16_t channel,uint16_t period,uint16_t mode){
	pwm_channel = channel;
	pwm_tim = tim;
	pwm_period = period;
	pwm_mode = mode;
	if(pwm_tim == TIM1){
		return 1;
	}else if(pwm_tim == TIM2){
		TIM2PWMSetup(pwm_period);
	}else if(pwm_tim == TIM3){
		TIM3PWMSetup(pwm_period);
	}else if(pwm_tim == TIM4){
		TIM4PWMSetup(pwm_period);
	}else{
		return 1;
	}

	switch(pwm_channel){
		case 1:
			OC1PWMSetup(pwm_tim,pwm_mode);
			break;
		case 2:
			OC2PWMSetup(pwm_tim,pwm_mode);
			break;
		case 3:
			OC3PWMSetup(pwm_tim,pwm_mode);
			break;
		case 4:
			OC4PWMSetup(pwm_tim,pwm_mode);
			break;
		default:
			return 1;
	}

	duty(0);
	return 0;
}

uint16_t PWM::duty(uint16_t duty){
	if(duty > pwm_period)duty = pwm_period;
	pwm_duty = duty;
	switch(pwm_channel){
		case 1:
			OC1DutySet(pwm_tim,pwm_duty);
			break;
		case 2:
			OC2DutySet(pwm_tim,pwm_duty);
			break;
		case 3:
			OC3DutySet(pwm_tim,pwm_duty);
			break;
		case 4:
			OC4DutySet(pwm_tim,pwm_duty);
			break;
		default:
			return 1;
	}
	return 0;
}


void TIM2PWMSetup(uint16_t period){
	RCC_APB1PeriphClockCmd(RCC_APB1ENR_TIM2EN,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2ENR_AFIOEN,ENABLE);

	TIM_TimeBaseInitTypeDef TimeBaseInitStructure;
	TIM_TimeBaseStructInit(&TimeBaseInitStructure);
	TimeBaseInitStructure.TIM_Period = period;
	TIM_TimeBaseInit(TIM2,&TimeBaseInitStructure);
}

void TIM3PWMSetup(uint16_t period){
	RCC_APB1PeriphClockCmd(RCC_APB1ENR_TIM3EN,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2ENR_AFIOEN,ENABLE);

	TIM_TimeBaseInitTypeDef TimeBaseInitStructure;
	TIM_TimeBaseStructInit(&TimeBaseInitStructure);
	TimeBaseInitStructure.TIM_Period = period;
	TIM_TimeBaseInit(TIM3,&TimeBaseInitStructure);
}

void TIM4PWMSetup(uint16_t period){
	RCC_APB1PeriphClockCmd(RCC_APB1ENR_TIM4EN,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2ENR_AFIOEN,ENABLE);

	TIM_TimeBaseInitTypeDef TimeBaseInitStructure;
	TIM_TimeBaseStructInit(&TimeBaseInitStructure);
	TimeBaseInitStructure.TIM_Period = period;
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

	TIM_ARRPreloadConfig(TIM3,ENABLE);
	TIM_Cmd(TIM3,ENABLE);
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




/********************************«ENCODER ªPWM***********************************************/

int32_t ENCODER::tim1Cnt = 0;
int32_t ENCODER::tim2Cnt = 0;
int32_t ENCODER::tim3Cnt = 0;
int32_t ENCODER::tim4Cnt = 0;


uint16_t ENCODER::setup(TIM_TypeDef *tim){
	enc_tim = tim;
	if(enc_tim == TIM1){
		TIM1_ENCODERSetup();
	}else if(enc_tim == TIM2){
		TIM2_ENCODERSetup();
	}else if(enc_tim == TIM3){
		TIM3_ENCODERSetup();
	}else if(enc_tim == TIM4){
		TIM4_ENCODERSetup();
	}else{
		return 1;
	}
	return 0;
}

int32_t ENCODER::read(){
	if(enc_tim == TIM1){
		return TIM1_ENCODERRead();
	}else if(enc_tim == TIM2){
		return TIM2_ENCODERRead();
	}else if(enc_tim == TIM3){
		return TIM3_ENCODERRead();
	}else if(enc_tim == TIM4){
		return TIM4_ENCODERRead();
	}else{
		return -1;
	}
}


void TIM1_ENCODERSetup(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	TIM_EncoderInterfaceConfig(TIM1,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM1,ENABLE);
}

void TIM2_ENCODERSetup(){
	RCC_APB1PeriphClockCmd(RCC_APB1ENR_TIM2EN,ENABLE);
	TIM_EncoderInterfaceConfig(TIM2,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM2,ENABLE);
}

void TIM3_ENCODERSetup(){
	RCC_APB1PeriphClockCmd(RCC_APB1ENR_TIM3EN,ENABLE);
	TIM_EncoderInterfaceConfig(TIM3,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM3,ENABLE);
}

void TIM4_ENCODERSetup(){
	RCC_APB1PeriphClockCmd(RCC_APB1ENR_TIM4EN,ENABLE);
	TIM_EncoderInterfaceConfig(TIM4,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM4,ENABLE);
}

int32_t TIM1_ENCODERRead(){
	return TIM1->CNT + ENCODER::tim1Cnt*65536;
}

int32_t TIM2_ENCODERRead(){
	return TIM2->CNT + ENCODER::tim2Cnt*65536;
}

int32_t TIM3_ENCODERRead(){
	return TIM3->CNT + ENCODER::tim3Cnt*65536;
}

int32_t TIM4_ENCODERRead(){
	return TIM4->CNT + ENCODER::tim4Cnt*65536;
}

void TIM1_ENCODER_IRQ(){
	if(TIM1->CNT < 32768){
		ENCODER::tim1Cnt ++;
	}else{
		ENCODER::tim1Cnt --;
	}
	TIM_ClearITPendingBit(TIM1,TIM_IT_Update);
}

void TIM2_ENCODER_IRQ(){

}

void TIM3_ENCODER_IRQ(){

}

void TIM4_ENCODER_IRQ(){

}





extern "C" void TIM1_IRQHandler(void){
	if(TIM_GetITStatus(TIM1,TIM_IT_Update) == SET){

	}
}


extern "C" void TIM2_IRQHandler(void){

	if(TIM2->CNT < 32768){
		ENCODER::tim2Cnt ++;
	}else{
		ENCODER::tim2Cnt --;
	}
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
}

extern "C" void TIM3_IRQHandler(void){
	if(TIM3->CNT < 32768){
		ENCODER::tim3Cnt ++;
	}else{
		ENCODER::tim3Cnt --;
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
}


extern "C" void TIM4_IRQHandler(void){
	if(TIM4->CNT < 32768){
		ENCODER::tim4Cnt ++;
	}else{
		ENCODER::tim4Cnt --;
	}
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
}
