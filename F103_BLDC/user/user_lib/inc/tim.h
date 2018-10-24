#ifndef TIM_H_
#define TIM_H_

#include "stm32f10x.h"
#include <stdio.h>

#include "systime.h"
#include "base.h"
#include "gpio.h"

#define PWM_PERIOD_DEFALUT 1024
#define TIM_OCMode_DEFAULT TIM_OCMode_PWM1

#define TIM_PWM 1
#define TIM_ENC 2
#define TIM_TIM 3

#define TIM1_ENC_Setup		TIM1,PA8,PA9
#define TIM2_ENC_Setup		TIM2,PA0,PA1
#define TIM3_ENC_Setup		TIM3,PA6,PA7
#define TIM4_ENC_Setup		TIM4,PB6,PB7
#define TIM2_ENC_RM_Setup	TIM2,PA15,PB3
#define TIM3_ENC_RM_Setup	TIM3,PB4,PB5

#define TIM1_PWM1_Setup		TIM1,1,PA8
#define TIM1_PWM2_Setup		TIM1,2,PA9
#define TIM1_PWM3_Setup		TIM1,3,PA10
#define TIM1_PWM4_Setup		TIM1,4,PA11

#define TIM2_PWM1_Setup		TIM2,1,PA0
#define TIM2_PWM2_Setup		TIM2,2,PA1
#define TIM2_PWM3_Setup		TIM2,3,PA2
#define TIM2_PWM4_Setup		TIM2,4,PA3

#define TIM3_PWM1_Setup		TIM3,1,PA6
#define TIM3_PWM2_Setup		TIM3,2,PA7
#define TIM3_PWM3_Setup		TIM3,3,PB0
#define TIM3_PWM4_Setup		TIM3,4,PB1

#define TIM4_PWM1_Setup		TIM4,1,PB6
#define TIM4_PWM2_Setup		TIM4,2,PB7
#define TIM4_PWM3_Setup		TIM4,3,PB8
#define TIM4_PWM4_Setup		TIM4,4,PB9

#define TIM2_PWM1_RM_Setup	TIM2,1,PA15
#define TIM2_PWM2_RM_Setup	TIM2,2,PB3
#define TIM2_PWM3_RM_Setup	TIM2,3,PB10
#define TIM2_PWM4_RM_Setup	TIM2,4,PB11

#define TIM3_PWM1_RM_Setup	TIM3,1,PB4
#define TIM3_PWM2_RM_Setup	TIM3,2,PB5

class TIM:public CountIn{
public:
	void encoderSetup(TIM_TypeDef* tim,GPIO_TypeDef* gpio1,uint16_t pin1,GPIO_TypeDef* gpio2,uint16_t pin2);
	int32_t read();
	void reset();
	void reverse(int8_t dir = -1);
	void write(int64_t cnt);

	int8_t encoder_dir;

	static int32_t tim1Cnt;
	static int32_t tim2Cnt;
	static int32_t tim3Cnt;
	static int32_t tim4Cnt;

	void pwmSetup(TIM_TypeDef* tim,uint16_t channel,GPIO_TypeDef* gpio,uint16_t pin,uint16_t period = PWM_PERIOD_DEFALUT,uint16_t prescaler = 0,uint16_t mode = TIM_OCMode_DEFAULT);
	void pwmReset(uint16_t period = PWM_PERIOD_DEFALUT,uint16_t prescaler = 0,uint16_t mode = TIM_OCMode_DEFAULT);
	uint16_t duty(uint16_t duty);
	uint16_t dutyF(float duty);
	void itSetup(uint16_t tim_it = TIM_IT_Update);

	void timerSetup(TIM_TypeDef* tim);
	uint64_t millis();
	uint64_t micros();

	uint16_t pwm_duty;
	uint16_t pwm_period;
	uint16_t pwm_prescaler;

	static uint32_t tim1_mode;
	static uint32_t tim2_mode;
	static uint32_t tim3_mode;
	static uint32_t tim4_mode;


	void ioSetupEnc(GPIO_TypeDef* gpio1,uint16_t pin1,GPIO_TypeDef* gpio2,uint16_t pin2);
	void ioSetupPwm(GPIO_TypeDef* gpio,uint16_t pin);

	TIM_TypeDef* tim_tim;
	uint16_t pwm_channel;
	uint16_t pwm_mode;
};


void TIM1Setup(uint16_t period = PWM_PERIOD_DEFALUT,uint16_t prescaler = 0);
void TIM2Setup(uint16_t period = PWM_PERIOD_DEFALUT,uint16_t prescaler = 0);
void TIM3Setup(uint16_t period = PWM_PERIOD_DEFALUT,uint16_t prescaler = 0);
void TIM4Setup(uint16_t period = PWM_PERIOD_DEFALUT,uint16_t prescaler = 0);

void TIM1ITSetup(uint16_t tim_it = TIM_IT_Update);
void TIM2ITSetup(uint16_t tim_it = TIM_IT_Update);
void TIM3ITSetup(uint16_t tim_it = TIM_IT_Update);
void TIM4ITSetup(uint16_t tim_it = TIM_IT_Update);

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


void TIM1EncoderSetup();
void TIM2EncoderSetup();
void TIM3EncoderSetup();
void TIM4EncoderSetup();

int32_t TIM1Read();
int32_t TIM2Read();
int32_t TIM3Read();
int32_t TIM4Read();

void TIM1_ENCODER_IRQ();
void TIM2_ENCODER_IRQ();
void TIM3_ENCODER_IRQ();
void TIM4_ENCODER_IRQ();

void TIM1_PWM_Update_IRQ();
void TIM2_PWM_Update_IRQ();
void TIM3_PWM_Update_IRQ();
void TIM4_PWM_Update_IRQ();

void TIM3_PWM_CC1_IRQ();
void TIM3_PWM_CC2_IRQ();
void TIM3_PWM_CC3_IRQ();
void TIM3_PWM_CC4_IRQ();

void TIM4_PWM_CC1_IRQ();
void TIM4_PWM_CC2_IRQ();
void TIM4_PWM_CC3_IRQ();
void TIM4_PWM_CC4_IRQ();

void TIM1_TIM_Update_IRQ();
void TIM2_TIM_Update_IRQ();
void TIM3_TIM_Update_IRQ();
void TIM4_TIM_Update_IRQ();


#endif
