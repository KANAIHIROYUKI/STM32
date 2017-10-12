#ifndef _KRM_H_
#define _KRM_H_

#include "system.h"
#include "app.h"

#define KRM_STICK_TOLERANCE 10
#define KRM_STICK_MAX 640

#define KRM_CHANNEL_REV  3			//SBUS�̃`�����l��
#define KRM_CHANNEL_RUN  1
#define KRM_CHANNEL_MODE 2
#define KRM_CHANNEL_ARM  5

#define KRM_Motor_Arm 0
#define KRM_Motor_Right 1
#define KRM_Motor_Left  2

#define KRM_MODE_STOP		0		//���샂�[�h
#define KRM_MODE_TEST		1
#define KRM_MODE_RUN		2
#define KRM_MODE_TIMEOUT	3

#define KRM_PRINT_CYCLE				100	//�v�����g����
#define KRM_SBUS_TIMEOUT_TOLERANCE	100	//SBUS�̃^�C���A�E�g
#define KRM_SA_TIMEOUT_TOLERANCE	100 //SerialArduino�̃^�C���A�E�g

#define KRM_ENC_REV_READ_INT 10		//�G���R�[�_�̉�]���ǂނ��߂ɃG���R�[�_�̒l�ǂފԊu

#define YAW 0
#define PITCH 1
#define ROLL 2

class KawaRobo {
public:
	void setup(USART &serial,SBUS &sbus,SerialArduino &sa,NCP5359 &motor0,NCP5359 &motor1,NCP5359 &motor2,NCP5359 &motor3);
	void uiSetup(Switch &sw0,Switch sw1,LED &led0,LED &led1,LED &led2,LED &led3);

	void cycle();
	void controlCycle();
	void run();
	void test();

	void display();

	void motorDisable();
	void motorEnable();

	float runRead();
	float revRead();
	float armRead();
	int stickRead(uint16_t channel,uint16_t offset = 1024);

	int16_t forward,revolution,armPower;
	int16_t gyroSpeed,dispValue;

	uint16_t mode,printValueSelect,ledIntervalTime[4],motorInvertFlag;
	uint64_t printTime,revReadTime,ledInterval[4],motorInvertTime;

private:
	USART *serial;
	NCP5359 *motor[4];
	SerialArduino *sa;
	SBUS *sbus;

	LED *led[4];
	Switch *sw[2];

	TIM enc;
	AS504x mgEnc;

	PID pid[4],target[4],robotX,robotY,robotR;
};


#endif
