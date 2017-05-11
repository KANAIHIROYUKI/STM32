#ifndef CAN_H_
#define CAN_H_

#include "system.h"


#define CAN_ADD_EMG 0x001

#define CAN_ADD_VOLTAGE_SETUP 		(uint16_t)0x050
#define CAN_ADD_VOLTAGE_VALUE 		(uint16_t)0x051

#define CAN_ADD_DUTY 				(uint16_t)0x100
#define CAN_ADD_FREE 				(uint16_t)0x120
#define CAN_ADD_CURRENT 			(uint16_t)0x140
#define CAN_ADD_POSITION 			(uint16_t)0x160
#define CAN_ADD_VELOCITY 			(uint16_t)0x180
#define CAN_ADD_SETTING 			(uint16_t)0x1A0
#define CAN_ADD_GET_VALUES 			(uint16_t)0x1C0
#define CAN_ADD_DEBUG 				(uint16_t)0x1E0


#define CAN_ADD_SWITCH_VALUE 		(uint16_t)0x240
#define CAN_ADD_SWITCH_SETUP 		(uint16_t)0x200

#define CAN_ADD_VALVE 				(uint16_t)0x280
#define CAN_ADD_ENC_SETUP 			(uint16_t)0x400
#define CAN_ADD_ENC_VALUE 			(uint16_t)0x440

class CAN{
public:
	CAN_TypeDef* can_can;
	//CanRxMsg rxMessage;
	void debug();
	void setup(CAN_TypeDef* can,GPIO_TypeDef* gpio_tx,uint16_t pin_tx,GPIO_TypeDef* gpio_rx,uint16_t pin_rx);
	void send(uint16_t id,uint8_t length,uint8_t *data);
	void filterAdd(uint16_t id1 = 0,uint16_t id2 = 0,uint16_t id3 = 0,uint16_t id4 = 0);
	void receive(CanRxMsg* canRxMessage);
	void receive();

	uint16_t receiveCnt;

	static uint16_t filterCnt;
	static uint16_t filterAddress[4*13];

	uint64_t lastReceiveTime;

	CanRxMsg rxMessage;

private:
	void ioSetup(GPIO_TypeDef* gpio_tx,uint16_t pin_tx,GPIO_TypeDef* gpio_rx,uint16_t pin_rx);
};


void CAN1Setup(uint16_t mode = 0);
void CAN1Send(uint16_t id,uint8_t length,uint8_t *data);
void CANFilterAdd(uint16_t id1 = 0,uint16_t id2 = 0,uint16_t id3 = 0,uint16_t id4 = 0);

#endif
