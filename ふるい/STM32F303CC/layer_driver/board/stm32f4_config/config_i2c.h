#ifndef CONFIG_I2C_H
#define CONFIG_I2C_H

#include "cmsis_lib/include/stm32f4xx_i2c.h"
#include "stm32f4xx.h"

void Init_i2c(I2C_TypeDef *use_i2c,GPIO_TypeDef *GPIOx, uint16_t sclPin, uint16_t sdaPin);


#endif//CONFIG_I2C_H
