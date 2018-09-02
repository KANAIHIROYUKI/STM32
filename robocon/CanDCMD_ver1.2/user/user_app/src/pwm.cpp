#include "pwm.h"

void Pwm::setup(TIM_TypeDef* tim,uint16_t channel,GPIO_TypeDef* gpio,uint16_t pin,uint16_t period,uint16_t prescaler,uint16_t mode){
	pwm_tim->pwmSetup(tim,channel,gpio,period,prescaler,mode);
}

void Pwm::duty(float duty){
	pwm_tim->duty(duty * pwm_tim->pwm_period);
}

void Pwm::reset(){
	pwm_tim->duty(0);
}
void Pwm::invert(int8_t mode){

}
