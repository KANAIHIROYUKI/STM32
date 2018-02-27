#ifndef CAN_H_
#define CAN_H_

#include "stm32f10x.h"
#include <stdio.h>

#include "gpio.h"
#include "systime.h"


#define CAN_INTERRUPT_FUNC_MAX 32


#define CAN_ADD_EMG 				(uint16_t)0x001

#define CAN_ADD_DUTY 				(uint16_t)0x100
#define CAN_ADD_FREE 				(uint16_t)0x120
#define CAN_ADD_CURRENT 			(uint16_t)0x140
#define CAN_ADD_POSITION 			(uint16_t)0x160
#define CAN_ADD_VELOCITY 			(uint16_t)0x180
#define CAN_ADD_SETTING 			(uint16_t)0x1A0
#define CAN_ADD_GET_VALUES 			(uint16_t)0x1C0
#define CAN_ADD_DEBUG 				(uint16_t)0x1E0

#define CAN_ADD_SWITCH_SETUP 		(uint16_t)0x200
#define CAN_ADD_SWITCH_VALUE 		(uint16_t)0x240

#define CAN_ADD_VALVE 				(uint16_t)0x280

#define CAN_ADD_VOLTAGE_SETUP 		(uint16_t)0x300
#define CAN_ADD_VOLTAGE_VALUE 		(uint16_t)0x340

#define CAN_ADD_ENC_SETUP 			(uint16_t)0x400
#define CAN_ADD_ENC_VALUE 			(uint16_t)0x440

class CAN{
public:
	//debugMode = 1にするとループバックモードになる
	void setup(CAN_TypeDef* can
			,GPIO_TypeDef* gpio_tx,uint16_t pin_tx
			,GPIO_TypeDef* gpio_rx,uint16_t pin_rx
			,uint16_t debugMode = 0);

	//送信､length=0にするとRTRに設定できる｡なおF103ではなぜかRTRが受信できない(試したり調べたけど不明のまま)
	void send(uint16_t id,uint8_t length,uint8_t *data);

	//フィルタ追加(4個づつ)
	void filterAdd(uint16_t id1,uint16_t id2,uint16_t id3 = 0,uint16_t id4 = 0);
	void filterAdd(uint16_t id1);

	//受信データを外部のcanRxMessageに入れる
	void receive(CanRxMsg* canRxMessage);

	//受信データをCANクラスの持つrxMessageに入れる
	void receive();
	CanRxMsg rxMessage;


	//受信回数カウント(わりとデバッグ用
	uint16_t receiveCnt;

	//送信エラー回数､エラー時の宛先アドレス､送信するアドレス(わりとデバッグ用
	static uint16_t errorCnt,errorAddress,sendAddress;
	static uint16_t filterCnt,filterAddress[4*14];

	//タイムスタンプ用(わりとデバッグ用
	uint64_t lastReceiveTime;

	CAN_TypeDef* can_can;

private:
	void ioSetup(GPIO_TypeDef* gpio_tx,uint16_t pin_tx,GPIO_TypeDef* gpio_rx,uint16_t pin_rx);
};

void CAN1Setup(uint16_t mode = 0);
void CAN1Send(uint16_t id,uint8_t length,uint8_t *data);
void CANFilterAdd(uint16_t id1 = 0,uint16_t id2 = 0,uint16_t id3 = 0,uint16_t id4 = 0);

//void CAN1AtachInterruptFunc(void (*po)());

#endif
