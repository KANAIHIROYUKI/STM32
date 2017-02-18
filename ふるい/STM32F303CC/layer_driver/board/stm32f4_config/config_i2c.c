#include "config_i2c.h"
#include "mcutime.h"
#include "name_converter.h"
#include <string.h>
#include <stdio.h>
#include <math.h>


void Init_i2c(I2C_TypeDef *use_i2c,GPIO_TypeDef *GPIOx, uint16_t sclPin, uint16_t sdaPin){
	GPIO_InitTypeDef GPIO_InitStructure;
	I2C_InitTypeDef	I2C_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	if(use_i2c == I2C1){
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);
	}else if(use_i2c == I2C2){
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2,ENABLE);
	}
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	GPIO_StructInit(&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = sclPin | sdaPin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

	if(use_i2c == I2C1){
		GPIO_Init(GPIOx, &GPIO_InitStructure);
		GPIO_PinAFConfig(GPIOx, Pin_select_source(sclPin), GPIO_AF_I2C1);
		GPIO_PinAFConfig(GPIOx,  Pin_select_source(sdaPin), GPIO_AF_I2C1);
	}else if(use_i2c == I2C2){
		GPIO_Init(GPIOx, &GPIO_InitStructure);
		GPIO_PinAFConfig(GPIOx, Pin_select_source(sclPin), GPIO_AF_I2C2);
		GPIO_PinAFConfig(GPIOx, Pin_select_source(sdaPin), GPIO_AF_I2C2);
	}
	I2C_ITConfig(use_i2c, I2C_IT_EVT|I2C_IT_BUF|I2C_IT_ERR, ENABLE);
	I2C_StructInit(&I2C_InitStructure);
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_OwnAddress1 = 0x00;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_ClockSpeed = 50000;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;

	I2C_Cmd(use_i2c,ENABLE);

	I2C_Init(use_i2c,&I2C_InitStructure);

	/* Configure the I2C event priority */
	if(use_i2c==I2C1){
		NVIC_InitStructure.NVIC_IRQChannel = I2C1_EV_IRQn;//|I2C1_ER_IRQn;
	}else if(use_i2c==I2C2){
		NVIC_InitStructure.NVIC_IRQChannel = I2C2_EV_IRQn;//|I2C2_ER_IRQn;
	}
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

