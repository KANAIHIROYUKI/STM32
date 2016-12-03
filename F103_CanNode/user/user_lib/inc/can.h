#ifndef CAN_H_
#define CAN_H_

#include "stm32f10x.h"
#include "stdio.h"

class CAN{
public:
	CAN_TypeDef* can_can;
	//CanRxMsg rxMessage;

	void setup(CAN_TypeDef* can = CAN1);
	void send(uint16_t id,uint8_t length,uint8_t *data);
	void filterAdd(uint16_t id1 = 0,uint16_t id2 = 0,uint16_t id3 = 0,uint16_t id4 = 0);
	void receive(CanRxMsg* canRxMessage);
	//void receive();

private:
};


void CAN1Setup();
void CAN1Send(uint16_t id,uint8_t length,uint8_t *data);
void CANFilterAdd(uint16_t id1 = 0,uint16_t id2 = 0,uint16_t id3 = 0,uint16_t id4 = 0);

#endif
