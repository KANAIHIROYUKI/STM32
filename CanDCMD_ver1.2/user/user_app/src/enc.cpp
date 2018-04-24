#include "enc.h"

void Enc::setup(TIM_TypeDef* tim,GPIO_TypeDef* gpio1,uint16_t pin1,GPIO_TypeDef* gpio2,uint16_t pin2){
	this->enc_tim->encoderSetup(tim,gpio1,pin1,gpio2,pin2);
}

int64_t Enc::read(){
	return enc_tim->read();
}
