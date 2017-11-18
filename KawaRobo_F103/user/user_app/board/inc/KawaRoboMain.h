#ifndef _KRM_H_
#define _KRM_H_

#include "system.h"
#include "app.h"

#define STICK_TOLERANCE 10
#define STICK_MAX 640

#define CHANNEL_REV		3			//SBUSのチャンネル
#define CHANNEL_RUN		1
#define CHANNEL_MODE	2
#define CHANNEL_ARM		2
#define CHANNEL_SEL		0

#define CHANNEL_TOGGLE0 2
#define CHANNEL_TOGGLE1 4
#define CHANNEL_TOGGLE2 5
#define CHANNEL_TOGGLE3 6
#define CHANNEL_VOLUME  7

#define Motor_Arm 1
#define Motor_Right 0
#define Motor_Left  3

#define MODE_STOP		0		//動作モード
#define MODE_TEST		1
#define MODE_RUN		2
#define MODE_TIMEOUT	3

#define PRINT_CYCLE				100	//プリント周期
#define SBUS_TIMEOUT_TOLERANCE	100	//SBUSのタイムアウト
#define SA_TIMEOUT_TOLERANCE	100 //SerialArduinoのタイムアウト
#define SA_EN_INTERVAL 			50

#define BATT_UNDER_LIMIT 	13200	//13.2V	動作下限電圧､3.3V/cell
#define BATT_UNDER_HYS		1000	//1V	ヒステリシス

#define YAW 0
#define PITCH 1
#define ROLL 2

#define MOTOR_OUT_REG_OFFSET 0.1
#define MOTOR_OUT_ARM_OFFSET 0.1

#define ATM_MAX_DEG  130		//機構限界
#define ARM_TOP_DEG  120		//跳ね上げ時にあげて意味のある角度
#define ARM_DEF_DEG  0			//初期角度(跳ね上げたあとに戻る
#define ARM_BTM_DEG -120		//最小角度(要らない気もする
#define ARM_MIN_DEG -130		//機構限界

class KawaRobo {
public:
	void setup(USART &serial,SBUS &sbus,SerialArduino &sa,NCP5359 &motor0,NCP5359 &motor1,NCP5359 &motor2,NCP5359 &motor3);
	void uiSetup(Switch &sw0,Switch &sw1,LED &led0,LED &led1,LED &led2,LED &led3);
	void sensorSetup(ADC &adc0,ADC &adc1,ADC &adc2,ADC &adc3,int16_t gyroOffset,float adcBatt);

	void cycle();
	void armPotCycle();
	void displayCycle();

	void controlCycle();

	void run();
	void test();

	void motorDisable();
	void motorEnable();

	float getRunPosition();
	float getArmPosition();
	float getRevPosition();
	float getSelectPosition();
	float getTogglePosition(uint16_t num);
	float getVolumePosition();
	int getStickPosition(uint16_t channel,uint16_t offset = 1024);

	void armPotUpdate();

	float armDegree,pot1Int,pot2Int,armTargetDeg;
	int pot1Cnt,pot2Cnt,potCnt;

	int16_t dispValue,gyroOffset;

	float adcToBattV;
	int speakRequest;
	uint16_t mode,printValueSelect,selectToggle,battUnderVoltage,motorEN;
	uint64_t printTime,controlCycleIntervalTime,saSendTime;


private:
	USART *serial;
	NCP5359 *motor[4];
	SerialArduino *sa;
	SBUS *sbus;

	LED *led[4];
	Switch *sw[2];

	ADC *analog[4];

	TIM enc;
	AS504x mgEnc;

	PID robotR,robotTargetR,robotPIDR,armPID,armTarget;
	PID armCurrent;
};


#endif
