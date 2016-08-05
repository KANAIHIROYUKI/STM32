#ifndef TIM_H_
#define TIM_H_

#define PWM_PERIOD_DEFALUT 1024

#include "stm32f10x.h"
#include "stdio.h"

class PWM{
public:
	uint16_t setup(TIM_TypeDef* tim,uint16_t channel,uint16_t period = PWM_PERIOD_DEFALUT,uint16_t mode = TIM_OCMode_PWM1);
	uint16_t duty(uint16_t duty);

	uint16_t pwm_duty;
	uint16_t pwm_period;

private:
	TIM_TypeDef* pwm_tim;
	uint16_t pwm_channel;
	uint16_t pwm_mode;

};



void TIM2PWMSetup(uint16_t period);
void TIM3PWMSetup(uint16_t period);
void TIM4PWMSetup(uint16_t period);

void TIM_OCStructInit_PWM(TIM_OCInitTypeDef *str);

void OC1PWMSetup(TIM_TypeDef*tim,uint16_t mode);
void OC2PWMSetup(TIM_TypeDef*tim,uint16_t mode);
void OC3PWMSetup(TIM_TypeDef*tim,uint16_t mode);
void OC4PWMSetup(TIM_TypeDef*tim,uint16_t mode);

void OC1DutySet(TIM_TypeDef*TIMx,uint16_t duty);
void OC2DutySet(TIM_TypeDef*TIMx,uint16_t duty);
void OC3DutySet(TIM_TypeDef*TIMx,uint16_t duty);
void OC4DutySet(TIM_TypeDef*TIMx,uint16_t duty);

/********************************Å´ENCODER Å™PWM***********************************************/

class ENCODER{
public:
	uint16_t setup(TIM_TypeDef* tim);
	int32_t read();
	static int32_t tim1Cnt;
	static int32_t tim2Cnt;
	static int32_t tim3Cnt;
	static int32_t tim4Cnt;
private:
	TIM_TypeDef *enc_tim;
};

void TIM1_ENCODERSetup();
void TIM2_ENCODERSetup();
void TIM3_ENCODERSetup();
void TIM4_ENCODERSetup();

int32_t TIM1_ENCODERRead();
int32_t TIM2_ENCODERRead();
int32_t TIM3_ENCODERRead();
int32_t TIM4_ENCODERRead();

#endif
