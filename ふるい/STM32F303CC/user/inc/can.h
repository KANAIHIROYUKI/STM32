#ifndef CAN_H_
#define CAN_H_

#include "stm32f30x.h"
#include "stdio.h"

void CAN1Setup();
void CAN1Send(uint16_t id,uint8_t length,uint8_t *data);
void CAN1FilterAdd(uint16_t id1 = 0,uint16_t id2 = 0,uint16_t id3 = 0,uint16_t id4 = 0);

#endif
