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
	//debugMode = 1�ɂ���ƃ��[�v�o�b�N���[�h�ɂȂ�
	void setup(CAN_TypeDef* can
			,GPIO_TypeDef* gpio_tx,uint16_t pin_tx
			,GPIO_TypeDef* gpio_rx,uint16_t pin_rx
			,uint16_t debugMode = 0);

	//���M�length=0�ɂ����RTR�ɐݒ�ł��顂Ȃ�F103�ł͂Ȃ���RTR����M�ł��Ȃ�(�������蒲�ׂ����Ǖs���̂܂�)
	void send(uint16_t id,uint8_t length,uint8_t *data);

	//�t�B���^�ǉ�(4�Â�)
	void filterAdd(uint16_t id1,uint16_t id2,uint16_t id3 = 0,uint16_t id4 = 0);
	void filterAdd(uint16_t id1);

	//��M�f�[�^���O����canRxMessage�ɓ����
	void receive(CanRxMsg* canRxMessage);

	//��M�f�[�^��CAN�N���X�̎���rxMessage�ɓ����
	void receive();
	CanRxMsg rxMessage;


	//��M�񐔃J�E���g(���ƃf�o�b�O�p
	uint16_t receiveCnt;

	//���M�G���[�񐔤�G���[���̈���A�h���X����M����A�h���X(���ƃf�o�b�O�p
	static uint16_t errorCnt,errorAddress,sendAddress;
	static uint16_t filterCnt,filterAddress[4*14];

	//�^�C���X�^���v�p(���ƃf�o�b�O�p
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
