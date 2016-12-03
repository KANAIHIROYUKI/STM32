#ifndef TIM_H_
#define TIM_H_

#include "stm32f10x.h"
#include "stdio.h"

#define PWM_PERIOD_DEFALUT 1024

#define TIM_PWM 1
#define TIM_ENC 2

class TIM{
public:
	void encoderSetup(TIM_TypeDef* tim);
	int32_t read();
	static int32_t tim1Cnt;
	static int32_t tim2Cnt;
	static int32_t tim3Cnt;
	static int32_t tim4Cnt;


	void pwmSetup(TIM_TypeDef* tim,uint16_t channel,uint16_t period = PWM_PERIOD_DEFALUT,uint16_t mode = TIM_OCMode_PWM1);
	uint16_t duty(uint16_t duty);
	void itSetup();

	uint16_t pwm_duty;
	uint16_t pwm_period;

	static uint32_t tim1_pwm_enc;
	static uint32_t tim2_pwm_enc;
	static uint32_t tim3_pwm_enc;
	static uint32_t tim4_pwm_enc;

private:
	TIM_TypeDef* pwm_enc_tim;
	uint16_t pwm_channel;
	uint16_t pwm_mode;
};


void TIM1PWMSetup(uint16_t period);
void TIM2PWMSetup(uint16_t period);
void TIM3PWMSetup(uint16_t period);
void TIM4PWMSetup(uint16_t period);

void TIM1ITSetup();
void TIM2ITSetup();
void TIM3ITSetup();
void TIM4ITSetup();

void TIM_OCStructInit_PWM(TIM_OCInitTypeDef *str);

void OC1PWMSetup(TIM_TypeDef*tim,uint16_t mode);
void OC2PWMSetup(TIM_TypeDef*tim,uint16_t mode);
void OC3PWMSetup(TIM_TypeDef*tim,uint16_t mode);
void OC4PWMSetup(TIM_TypeDef*tim,uint16_t mode);

void OC1DutySet(TIM_TypeDef*TIMx,uint16_t duty);
void OC2DutySet(TIM_TypeDef*TIMx,uint16_t duty);
void OC3DutySet(TIM_TypeDef*TIMx,uint16_t duty);
void OC4DutySet(TIM_TypeDef*TIMx,uint16_t duty);

/********************************��ENCODER ��PWM***********************************************/

void TIM1_ENCODERSetup();
void TIM2_ENCODERSetup();
void TIM3_ENCODERSetup();
void TIM4_ENCODERSetup();

int32_t TIM1_ENCODERRead();
int32_t TIM2_ENCODERRead();
int32_t TIM3_ENCODERRead();
int32_t TIM4_ENCODERRead();

void TIM1_ENCODER_IRQ();
void TIM2_ENCODER_IRQ();
void TIM3_ENCODER_IRQ();
void TIM4_ENCODER_IRQ();

void TIM1_PWM_Update_IRQ();
void TIM2_PWM_Update_IRQ();
void TIM3_PWM_Update_IRQ();
void TIM4_PWM_Update_IRQ();



#endif