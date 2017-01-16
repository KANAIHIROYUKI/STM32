#ifndef CAN_H_
#define CAN_H_

#include "system.h"

#define CAN_ADD_MOTOR 0x100
#define CAN_ADD_VALVE 0x280
#define CAN_ADD_ENC_SETUP 0x400
#define CAN_ADD_ENC_VALUE 0x440

class CAN{
public:
	CAN_TypeDef* can_can;
	//CanRxMsg rxMessage;

	void setup(CAN_TypeDef* can,GPIO_TypeDef* gpio_tx,uint16_t pin_tx,GPIO_TypeDef* gpio_rx,uint16_t pin_rx);
	void send(uint16_t id,uint8_t length,uint8_t *data);
	void filterAdd(uint16_t id1 = 0,uint16_t id2 = 0,uint16_t id3 = 0,uint16_t id4 = 0);
	void receive(CanRxMsg* canRxMessage);
	//void receive();

	static uint16_t filterCnt;
	static uint16_t filterAddress[4*13];

private:
	void ioSetup(GPIO_TypeDef* gpio_tx,uint16_t pin_tx,GPIO_TypeDef* gpio_rx,uint16_t pin_rx);

};


void CAN1Setup();
void CAN1Send(uint16_t id,uint8_t length,uint8_t *data);
void CANFilterAdd(uint16_t id1 = 0,uint16_t id2 = 0,uint16_t id3 = 0,uint16_t id4 = 0);

#endif
