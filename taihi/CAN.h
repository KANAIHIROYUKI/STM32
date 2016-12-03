#ifndef CAN_H_
#define CAN_H_

#include "stm32f10x.h"
#include "stdio.h"

void CAN1Setup();
void CAN1Send(uint16_t id,uint8_t length,uint8_t *data);


#endif