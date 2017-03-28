#ifndef TIM_H_
#define TIM_H_

//#include "system.h"
#include "gpio.h"

#define PWM_PERIOD_DEFALUT 1024
#define TIM_OCMode_DEFAULT TIM_OCMode_PWM1

#define TIM_PWM 1
#define TIM_ENC 2
#define TIM_TIM 3

class TIM{
public:
	void encoderSetup(TIM_TypeDef* tim,GPIO_TypeDef* gpio1,uint16_t pin1,GPIO_TypeDef* gpio2,uint16_t pin2);
	int32_t read();
	void reset();
	void reverse(int8_t dir = -1);

	int8_t encoder_dir;

	static int32_t tim1Cnt;
	static int32_t tim2Cnt;
	static int32_t tim3Cnt;
	static int32_t tim4Cnt;

	void pwmSetup(TIM_TypeDef* tim,uint16_t channel,GPIO_TypeDef* gpio,uint16_t pin,uint16_t period = PWM_PERIOD_DEFALUT,uint16_t prescaler = 0,uint16_t mode = TIM_OCMode_DEFAULT);
	uint16_t duty(uint16_t duty);
	void itSetup();

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

private:
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

//割込みの設定関数｡何も入れない場合は勝手にカウント完了オーバーフロー割込みにしてくれる｡引数で割込みの種類を選択できる
void TIM1ITSetup(uint16_t tim_it = TIM_IT_Update);
void TIM2ITSetup(uint16_t tim_it = TIM_IT_Update);
void TIM3ITSetup(uint16_t tim_it = TIM_IT_Update);
void TIM4ITSetup(uint16_t tim_it = TIM_IT_Update);

//オシレータの設定関数｡PWMの設定しかできない｡
void TIM_OCStructInit_PWM(TIM_OCInitTypeDef *str);

void OC1PWMSetup(TIM_TypeDef*tim,uint16_t mode);
void OC2PWMSetup(TIM_TypeDef*tim,uint16_t mode);
void OC3PWMSetup(TIM_TypeDef*tim,uint16_t mode);
void OC4PWMSetup(TIM_TypeDef*tim,uint16_t mode);

void OC1DutySet(TIM_TypeDef*TIMx,uint16_t duty);
void OC2DutySet(TIM_TypeDef*TIMx,uint16_t duty);
void OC3DutySet(TIM_TypeDef*TIMx,uint16_t duty);
void OC4DutySet(TIM_TypeDef*TIMx,uint16_t duty);

/********************************↓ENCODER ↑PWM***********************************************/

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

void TIM4_PWM_CC1_IRQ();
void TIM4_PWM_CC2_IRQ();
void TIM4_PWM_CC3_IRQ();
void TIM4_PWM_CC4_IRQ();

void TIM1_TIM_Update_IRQ();
void TIM2_TIM_Update_IRQ();
void TIM3_TIM_Update_IRQ();
void TIM4_TIM_Update_IRQ();


#endif
