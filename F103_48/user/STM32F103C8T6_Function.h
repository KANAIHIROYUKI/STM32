#ifndef STM32F103C8T6_FUNC
#define STM32F103C8T6_FUNC

#include "stm32f10x.h"
#include "stdio.h"

#define ADC_Cycle ADC_SampleTime_13Cycles5


class GPIO {
public:
	void setup(GPIO_TypeDef* gpio,uint16_t pin,GPIOMode_TypeDef mode);
	uint16_t write(BitAction value);
	uint16_t read();

	static uint16_t s_gpioSetuped;

private:
	uint16_t gpio_pin;
	GPIO_TypeDef* gpio_gpio;
	GPIOMode_TypeDef gpio_mode;
};

void GPIOSetup();

void pinSetup(GPIO_TypeDef* gpio,uint16_t pin,GPIOMode_TypeDef mode);
void pinWrite(GPIO_TypeDef* gpio,uint16_t pin,BitAction status);

void ADC1Setup(uint8_t);
uint16_t ADC1Read(uint8_t);

void ADC2Setup(uint8_t);
uint16_t ADC2Read(uint8_t);

void TIM2PWMSetup(uint16_t count);
void TIM3PWMSetup(uint16_t count);
void TIM4PWMSetup(uint16_t count);

void TIM_OCStructInit_PWM(TIM_OCInitTypeDef *str);

void OC1PWMSetup(TIM_TypeDef*tim,uint16_t mode);
void OC2PWMSetup(TIM_TypeDef*tim,uint16_t mode);
void OC3PWMSetup(TIM_TypeDef*tim,uint16_t mode);
void OC4PWMSetup(TIM_TypeDef*tim,uint16_t mode);

void OC1DutySet(TIM_TypeDef*TIMx,uint16_t duty);
void OC2DutySet(TIM_TypeDef*TIMx,uint16_t duty);
void OC3DutySet(TIM_TypeDef*TIMx,uint16_t duty);
void OC4DutySet(TIM_TypeDef*TIMx,uint16_t duty);

void USART1Setup(uint32_t baud);
void USART1ITSetup(uint16_t flag);

void USART2Setup(uint32_t baud);
void USART2ITSetup(uint16_t flag);

void CAN1Setup();
void CAN1Send(uint16_t id,uint8_t length,uint8_t *data);

static uint32_t TimingDelay;
void delay(uint32_t nTime);


#endif
