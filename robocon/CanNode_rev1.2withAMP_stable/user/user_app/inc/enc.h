#ifndef _ENC_H_
#define _ENC_H_

#include "system.h"
#include "base.h"

class Enc{
public:
	void setup(TIM_TypeDef* tim,GPIO_TypeDef* gpio1,uint16_t pin1,GPIO_TypeDef* gpio2,uint16_t pin2);
	virtual int64_t read();
	virtual void invert();

	TIM* enc_tim;
};

#endif
