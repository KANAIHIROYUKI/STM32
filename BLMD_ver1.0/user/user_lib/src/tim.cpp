#include "tim.h"

//エンコーダかPWMか､PWM=1､ENC=2,TIM=3
uint32_t TIM::tim1_mode = 0;
uint32_t TIM::tim2_mode = 0;
uint32_t TIM::tim3_mode = 0;
uint32_t TIM::tim4_mode = 0;

//エンコーダカウント
int32_t TIM::tim1Cnt = 0;
int32_t TIM::tim2Cnt = 0;
int32_t TIM::tim3Cnt = 0;
int32_t TIM::tim4Cnt = 0;

void TIM::pwmSetup(TIM_TypeDef* tim,uint16_t channel,GPIO_TypeDef* gpio,uint16_t pin,uint16_t period,uint16_t prescaler,uint16_t mode){
	ioSetupPwm(gpio,pin);
	pwm_channel = channel;
	tim_tim = tim;
	pwm_period = period;
	pwm_prescaler = prescaler;
	pwm_mode = mode;
	if(tim_tim == TIM1){
		tim1_mode = TIM_PWM;
		TIM1Setup(pwm_period,pwm_prescaler);

	}else if(tim_tim == TIM2){
		tim2_mode = TIM_PWM;
		TIM2Setup(pwm_period,pwm_prescaler);

	}else if(tim_tim == TIM3){
		tim3_mode = TIM_PWM;
		TIM3Setup(pwm_period,pwm_prescaler);

	}else if(tim_tim == TIM4){
		tim4_mode = TIM_PWM;
		TIM4Setup(pwm_period,pwm_prescaler);

	}else{
		return;
	}
	TIM_ARRPreloadConfig(tim_tim,ENABLE);
	TIM_Cmd(tim_tim,ENABLE);

	switch(pwm_channel){
		case 1:
			OC1PWMSetup(tim_tim,pwm_mode);
			break;
		case 2:
			OC2PWMSetup(tim_tim,pwm_mode);
			break;
		case 3:
			OC3PWMSetup(tim_tim,pwm_mode);
			break;
		case 4:
			OC4PWMSetup(tim_tim,pwm_mode);
			break;
		default:
			return;
	}

	duty(0);
}

uint16_t TIM::duty(uint16_t duty){
	if(tim_tim == TIM1){
		if(tim1_mode != TIM_PWM)return 1;
	}else if(tim_tim == TIM2){
		if(tim2_mode != TIM_PWM)return 1;
	}else if(tim_tim == TIM3){
		if(tim3_mode != TIM_PWM)return 1;
	}else if(tim_tim == TIM4){
		if(tim4_mode != TIM_PWM)return 1;
	}

	if(duty > pwm_period)duty = pwm_period;
	pwm_duty = duty;
	switch(pwm_channel){
		case 1:
			OC1Set(tim_tim,pwm_duty);
			break;
		case 2:
			OC2Set(tim_tim,pwm_duty);
			break;
		case 3:
			OC3Set(tim_tim,pwm_duty);
			break;
		case 4:
			OC4Set(tim_tim,pwm_duty);
			break;
		default:
			return 1;
	}
	return 0;
}

void TIM::itSetup(uint16_t tim_it){
	if(tim_tim == TIM1){
		TIM1ITSetup(tim_it);
	}else if(tim_tim == TIM2){
		TIM2ITSetup(tim_it);
	}else if(tim_tim == TIM3){
		TIM3ITSetup(tim_it);
	}else if(tim_tim == TIM4){
		TIM4ITSetup(tim_it);
	}
	TIM_ITConfig(tim_tim,tim_it,ENABLE);
}


void TIM::encoderSetup(TIM_TypeDef *tim,GPIO_TypeDef* gpio1,uint16_t pin1,GPIO_TypeDef* gpio2,uint16_t pin2){
	ioSetupEnc(gpio1,pin1,gpio2,pin2);

	tim_tim = tim;
	encoder_dir = 1;
	if(tim_tim == TIM1){
		tim1_mode = TIM_ENC;
		TIM1EncoderSetup();

	}else if(tim_tim == TIM2){
		tim2_mode = TIM_ENC;
		TIM2EncoderSetup();

	}else if(tim_tim == TIM3){
		tim3_mode = TIM_ENC;
		TIM3EncoderSetup();

	}else if(tim_tim == TIM4){
		tim4_mode = TIM_ENC;
		TIM4EncoderSetup();
	}

	TIM_EncoderInterfaceConfig(tim_tim,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);
	TIM_ITConfig(tim_tim,TIM_IT_Update,ENABLE);

	TIM_ARRPreloadConfig(tim_tim,ENABLE);
	TIM_Cmd(tim_tim,ENABLE);
}

void TIM::ioSetupEnc(GPIO_TypeDef* gpio1,uint16_t pin1,GPIO_TypeDef* gpio2,uint16_t pin2){
	GPIO A,B;
	A.setup(gpio1,pin1,INPUT_PU);
	B.setup(gpio2,pin2,INPUT_PU);
	return;
}

void TIM::ioSetupPwm(GPIO_TypeDef* gpio,uint16_t pin){
	GPIO A;
	A.setup(gpio,pin,OUTPUT_AF);
	return;
}

int32_t TIM::read(){
	if(tim_tim == TIM1){
		return TIM1Read()*encoder_dir;
	}else if(tim_tim == TIM2){
		return TIM2Read()*encoder_dir;
	}else if(tim_tim == TIM3){
		return TIM3Read()*encoder_dir;
	}else if(tim_tim == TIM4){
		return TIM4Read()*encoder_dir;
	}else{
		return -1;
	}
}

void TIM::reset(){
	if(tim_tim == TIM1){
		tim1Cnt = 0;
		TIM1->CNT = 0;
	}else if(tim_tim == TIM2){
		tim2Cnt = 0;
		TIM2->CNT = 0;
	}else if(tim_tim == TIM3){
		tim3Cnt = 0;
		TIM3->CNT = 0;
	}else if(tim_tim == TIM4){
		tim4Cnt = 0;
		TIM4->CNT = 0;
	}
}

void TIM::reverse(int8_t dir){
	encoder_dir = dir;
}

/************************/

void TIM::timerSetup(TIM_TypeDef* tim){
	tim_tim = tim;
	encoder_dir = 1;

	if(tim_tim == TIM1){
		tim1_mode = TIM_TIM;
		TIM1Setup(65535,71);
	}else if(tim_tim == TIM2){
		tim2_mode = TIM_TIM;
		TIM2Setup(65535,71);
	}else if(tim_tim == TIM3){
		tim3_mode = TIM_TIM;
		TIM3Setup(65535,71);
	}else if(tim_tim == TIM4){
		tim4_mode = TIM_TIM;
		TIM4Setup(65535,71);
	}else{
		return;
	}
	itSetup();
}

void TIM::triggerSetup(TIM_TypeDef* tim,uint16_t channel,uint16_t triggerCnt){
	tim_tim = tim;
	pwm_channel = channel;
	encoder_dir = 1;	//これないとread()で読めない

	if(tim_tim == TIM1){
		tim1_mode = TIM_TRG;
		reset();
		TIM1Setup(65535,71);
	}else if(tim_tim == TIM2){
		tim2_mode = TIM_TRG;
		reset();
		TIM2Setup(65535,71);
	}else if(tim_tim == TIM3){
		tim3_mode = TIM_TRG;
		reset();
		TIM3Setup(65535,71);
	}else if(tim_tim == TIM4){
		tim4_mode = TIM_TRG;
		reset();
		TIM4Setup(65535,71);
	}else{
		return;
	}

	TIM_ARRPreloadConfig(tim_tim,DISABLE);
	TIM_SelectOnePulseMode(tim_tim,TIM_OPMode_Single);

	switch(channel){
	case 1:
		OC1Set(tim_tim,triggerCnt);
		itSetup(TIM_IT_CC1);
		break;
	case 2:
		OC2Set(tim_tim,triggerCnt);
		itSetup(TIM_IT_CC2);
		break;
	case 3:
		OC3Set(tim_tim,triggerCnt);
		itSetup(TIM_IT_CC3);
		break;
	case 4:
		OC4Set(tim_tim,triggerCnt);
		itSetup(TIM_IT_CC4);
		break;
	default:
		return;
	}
	reset();
}


void TIM::triggerStart(){

}

void TIM::triggerStop(){

}

/************************************************↑class member ↓function*************************************************************************/

void TIMxSetup(TIM_TypeDef* tim,uint16_t period,uint16_t prescaler){
	TIM_TimeBaseInitTypeDef TimeBaseInitStructure;
	TIM_TimeBaseStructInit(&TimeBaseInitStructure);
	TimeBaseInitStructure.TIM_Period = period;
	TimeBaseInitStructure.TIM_Prescaler = prescaler;
	TIM_TimeBaseInit(tim,&TimeBaseInitStructure);
}

void TIM1Setup(uint16_t period,uint16_t prescaler){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2ENR_AFIOEN,ENABLE);

	TIMxSetup(TIM1,period,prescaler);
}

void TIM2Setup(uint16_t period,uint16_t prescaler){
	RCC_APB1PeriphClockCmd(RCC_APB1ENR_TIM2EN,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2ENR_AFIOEN,ENABLE);

	TIM_TimeBaseInitTypeDef TimeBaseInitStructure;
	TIM_TimeBaseStructInit(&TimeBaseInitStructure);
	TimeBaseInitStructure.TIM_Period = period;
	TimeBaseInitStructure.TIM_Prescaler = prescaler;
	TIM_TimeBaseInit(TIM3,&TimeBaseInitStructure);
	//TIMxSetup(TIM2,period,prescaler);
}

void TIM3Setup(uint16_t period,uint16_t prescaler){
	RCC_APB1PeriphClockCmd(RCC_APB1ENR_TIM3EN,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2ENR_AFIOEN,ENABLE);

	TIMxSetup(TIM3,period,prescaler);
}

void TIM4Setup(uint16_t period,uint16_t prescaler){
	RCC_APB1PeriphClockCmd(RCC_APB1ENR_TIM4EN,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2ENR_AFIOEN,ENABLE);

	TIMxSetup(TIM4,period,prescaler);
}


void TIM1ITSetup(uint16_t tim_it){
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void TIM2ITSetup(uint16_t tim_it){
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void TIM3ITSetup(uint16_t tim_it){
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void TIM4ITSetup(uint16_t tim_it){
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/********************↑CC↓OC*****************************************/

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
}

void OC3PWMSetup(TIM_TypeDef *tim,uint16_t mode){
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = mode;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;//duty
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC3Init(tim,&TIM_OCInitStructure);
}

void OC4PWMSetup(TIM_TypeDef *tim,uint16_t mode){
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = mode;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;//duty
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC4Init(tim,&TIM_OCInitStructure);

}

void OC1Set(TIM_TypeDef*tim,uint16_t duty){
	tim->CCR1 = duty;
}

void OC2Set(TIM_TypeDef*tim,uint16_t duty){
	tim->CCR2 = duty;
}

void OC3Set(TIM_TypeDef*tim,uint16_t duty){
	tim->CCR3 = duty;
}

void OC4Set(TIM_TypeDef*tim,uint16_t duty){
	tim->CCR4 = duty;
}


/********************************↓ENCODER ↑PWM***********************************************/



void TIM1EncoderSetup(){
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
}

void TIM2EncoderSetup(){
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	RCC_APB1PeriphClockCmd(RCC_APB1ENR_TIM2EN,ENABLE);
}

void TIM3EncoderSetup(){
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	RCC_APB1PeriphClockCmd(RCC_APB1ENR_TIM3EN,ENABLE);
}

void TIM4EncoderSetup(){
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	RCC_APB1PeriphClockCmd(RCC_APB1ENR_TIM4EN,ENABLE);
}

int32_t TIM1Read(){
	return TIM1->CNT + TIM::tim1Cnt*65536;
}

int32_t TIM2Read(){
	return TIM2->CNT + TIM::tim2Cnt*65536;
}

int32_t TIM3Read(){
	return TIM3->CNT + TIM::tim3Cnt*65536;
}

int32_t TIM4Read(){
	return TIM4->CNT + TIM::tim4Cnt*65536;
}


/**************************************************↑function ↓interrupt***************************************************************************/


void TIM1_ENCODER_IRQ(){
	if(TIM1->CNT < 32768){
		TIM::tim1Cnt ++;
	}else{
		TIM::tim1Cnt --;
	}
}

void TIM2_ENCODER_IRQ(){
	if(TIM2->CNT < 32768){
		TIM::tim2Cnt ++;
	}else{
		TIM::tim2Cnt --;
	}
}

void TIM3_ENCODER_IRQ(){
	if(TIM3->CNT < 32768){
		TIM::tim3Cnt ++;
	}else{
		TIM::tim3Cnt --;
	}
}

void TIM4_ENCODER_IRQ(){
	if(TIM4->CNT < 32768){
		TIM::tim4Cnt ++;
	}else{
		TIM::tim4Cnt --;
	}
}


void TIM1_PWM_Update_IRQ(){

}

void TIM1_PWM_CC1_IRQ(){

}

void TIM1_PWM_CC2_IRQ(){

}

void TIM1_PWM_CC3_IRQ(){

}

void TIM1_PWM_CC4_IRQ(){

}

/****TIM1-TIM2****/

void TIM2_PWM_Update_IRQ(){

}

void TIM2_PWM_CC1_IRQ(){

}

void TIM2_PWM_CC2_IRQ(){

}

void TIM2_PWM_CC3_IRQ(){

}

void TIM2_PWM_CC4_IRQ(){

}

void TIM2_TIMER_Update_IRQ(){

}

/****TIM2-TIM3****/

void TIM3_PWM_Update_IRQ(){

}

void TIM3_PWM_CC1_IRQ(){

}

void TIM3_PWM_CC2_IRQ(){

}

void TIM3_PWM_CC3_IRQ(){

}

void TIM3_PWM_CC4_IRQ(){

}

/****TIM3-TIM4****/

void TIM4_PWM_Update_IRQ(){

}

void TIM4_PWM_CC1_IRQ(){

}

void TIM4_PWM_CC2_IRQ(){

}

void TIM4_PWM_CC3_IRQ(){

}

void TIM4_PWM_CC4_IRQ(){

}

void TIM1_TIM_Update_IRQ(){
	TIM::tim1Cnt++;
}

void TIM2_TIM_Update_IRQ(){
	TIM::tim2Cnt++;
}

void TIM3_TIM_Update_IRQ(){
	TIM::tim3Cnt++;
}

void TIM4_TIM_Update_IRQ(){
	TIM::tim4Cnt++;
}

/***************************TRIGGER TIMER************************************/

void TIM1_TRIGGER_Update_IRQ(){
	TIM::tim1Cnt ++;
}
/*
void TIM2_TRIGGER_Update_IRQ(){
	TIM::tim2Cnt ++;
}
*/
void TIM3_TRIGGER_Update_IRQ(){
	TIM::tim3Cnt ++;
}

void TIM4_TRIGGER_Update_IRQ(){
	TIM::tim4Cnt ++;
}

void TIM2_TRIGGER_CC1_IRQ(){

}

void TIM2_TRIGGER_CC2_IRQ(){

}

void TIM2_TRIGGER_CC3_IRQ(){

}

void TIM2_TRIGGER_CC4_IRQ(){

}

extern "C" void TIM1_UP_IRQHandler(void){
	if(TIM::tim1_mode == TIM_ENC){
		TIM1_ENCODER_IRQ();
	}else if(TIM::tim1_mode == TIM_PWM){
		TIM1_PWM_Update_IRQ();
	}else if(TIM::tim1_mode == TIM_TIM){
		TIM1_TIM_Update_IRQ();
	}else if(TIM::tim1_mode == TIM_TRG){
		TIM1_TRIGGER_Update_IRQ();
		TIM_ClearITPendingBit(TIM1,TIM_IT_Update);
		TIM_Cmd(TIM4,DISABLE);
		TIM_ITConfig(TIM1,TIM_IT_Update,DISABLE);
	}
	TIM_ClearITPendingBit(TIM1,TIM_IT_Update);
}


extern "C" void TIM2_IRQHandler(void){
	if(TIM::tim2_mode == TIM_ENC){
		TIM2_ENCODER_IRQ();
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	}else if(TIM::tim2_mode == TIM_PWM){
		if(TIM_GetITStatus(TIM2,TIM_IT_Update) == SET){
			TIM2_PWM_Update_IRQ();
			TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
		}else if(TIM_GetITStatus(TIM2,TIM_IT_CC1) == SET){
			TIM2_PWM_CC1_IRQ();
			TIM_ClearITPendingBit(TIM2,TIM_IT_CC1);
		}else if(TIM_GetITStatus(TIM2,TIM_IT_CC2) == SET){
			TIM2_PWM_CC2_IRQ();
			TIM_ClearITPendingBit(TIM2,TIM_IT_CC2);
		}else if(TIM_GetITStatus(TIM2,TIM_IT_CC3) == SET){
			TIM2_PWM_CC3_IRQ();
			TIM_ClearITPendingBit(TIM2,TIM_IT_CC3);
		}else if(TIM_GetITStatus(TIM2,TIM_IT_CC4) == SET){
			TIM2_PWM_CC4_IRQ();
			TIM_ClearITPendingBit(TIM2,TIM_IT_CC4);
		}

	}else if(TIM::tim2_mode == TIM_TIM){
		TIM2_TIM_Update_IRQ();
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	}else if(TIM::tim2_mode == TIM_TRG){
		if(TIM_GetITStatus(TIM2,TIM_IT_Update) == SET){
			TIM2_TRIGGER_Update_IRQ();
			TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
		}else if(TIM_GetITStatus(TIM2,TIM_IT_CC1) == SET){
			TIM2_TRIGGER_CC1_IRQ();
			TIM_ClearITPendingBit(TIM2,TIM_IT_CC1);
		}else if(TIM_GetITStatus(TIM2,TIM_IT_CC2) == SET){
			TIM2_TRIGGER_CC2_IRQ();
			TIM_ClearITPendingBit(TIM2,TIM_IT_CC2);
		}else if(TIM_GetITStatus(TIM2,TIM_IT_CC3) == SET){
			TIM2_TRIGGER_CC3_IRQ();
			TIM_ClearITPendingBit(TIM2,TIM_IT_CC3);
		}else if(TIM_GetITStatus(TIM2,TIM_IT_CC4) == SET){
			TIM2_TRIGGER_CC4_IRQ();
			TIM_ClearITPendingBit(TIM2,TIM_IT_CC4);
		}
		TIM_Cmd(TIM2,DISABLE);
		TIM_ITConfig(TIM2,TIM_IT_Update,DISABLE);
	}

}

extern "C" void TIM3_IRQHandler(void){
	if(TIM::tim3_mode == TIM_ENC){
		TIM3_ENCODER_IRQ();
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
	}else if(TIM::tim3_mode == TIM_PWM){
		TIM3_PWM_Update_IRQ();
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
	}else if(TIM::tim3_mode == TIM_TIM){
		TIM3_TIM_Update_IRQ();
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
	}else if(TIM::tim3_mode == TIM_TRG){
		TIM3_TRIGGER_Update_IRQ();
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
		TIM_Cmd(TIM3,DISABLE);
		TIM_ITConfig(TIM3,TIM_IT_Update,DISABLE);
	}
}


extern "C" void TIM4_IRQHandler(void){
	if(TIM::tim4_mode == TIM_ENC){
		TIM4_ENCODER_IRQ();
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
	}else if(TIM::tim4_mode == TIM_PWM){
		if(TIM_GetITStatus(TIM4,TIM_IT_Update) == SET){
			TIM4_PWM_Update_IRQ();
			TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
		}else if(TIM_GetITStatus(TIM4,TIM_IT_CC1) == SET){
			TIM4_PWM_CC1_IRQ();
			TIM_ClearITPendingBit(TIM4,TIM_IT_CC1);

		}else if(TIM_GetITStatus(TIM4,TIM_IT_CC2) == SET){
			TIM4_PWM_CC2_IRQ();
			TIM_ClearITPendingBit(TIM4,TIM_IT_CC2);

		}else if(TIM_GetITStatus(TIM4,TIM_IT_CC3) == SET){
			TIM4_PWM_CC3_IRQ();
			TIM_ClearITPendingBit(TIM4,TIM_IT_CC3);

		}else if(TIM_GetITStatus(TIM4,TIM_IT_CC4) == SET){
			TIM4_PWM_CC4_IRQ();
			TIM_ClearITPendingBit(TIM4,TIM_IT_CC4);
		}
	}else if(TIM::tim4_mode == TIM_TIM){
		TIM4_TIM_Update_IRQ();
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
	}else if(TIM::tim4_mode == TIM_TRG){
		TIM4_TRIGGER_Update_IRQ();
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
		TIM_Cmd(TIM4,DISABLE);
		TIM_ITConfig(TIM4,TIM_IT_Update,DISABLE);
	}
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);

}
