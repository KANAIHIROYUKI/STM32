#ifndef _KRM_H_
#define _KRM_H_

#include "system.h"
#include "app.h"
#include "util.hpp"

#define STICK_TOLERANCE 10
#define STICK_MAX 640

#define CHANNEL_REV		3			//SBUSのチャンネル
#define CHANNEL_RUN		1
#define CHANNEL_MODE	2
#define CHANNEL_ARM		2
#define CHANNEL_SEL		0

#define TOGGLE_CONTROL_MODE 3
#define TOGGLE_POWER_LIMIT  2

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
#define MODE_DISCONNECT	3
#define MODE_ERROR		4

#define PRINT_CYCLE				100	//プリント周期
#define SBUS_TIMEOUT_TOLERANCE	100	//SBUSのタイムアウト
#define SA_TIMEOUT_TOLERANCE	100 //SerialArduinoのタイムアウト
#define SERIKA_TIME				10	//最初に喋る時間

#define SA_RECEIVE_OK millis() - sa->lastReceiveTime < SA_TIMEOUT_TOLERANCE

#define BATT_UNDER_LIMIT 	2.3*4*1000	//11.2V	動作下限電圧､2.8V/cell
#define BATT_UNDER_HYS		1000	//1V	ヒステリシス

#define YAW 0
#define PITCH 1
#define ROLL 2

#define MOTOR_OUT_REG_OFFSET 0.1
#define MOTOR_OUT_ARM_OFFSET 0.1

#define ATM_MAX_DEG  130		//機構限界
#define ARM_TOP_DEG  110		//跳ね上げ時にあげて意味のある角度
#define ARM_DEF_DEG  0			//初期角度(跳ね上げたあとに戻る
#define ARM_BTM_DEG -100		//最小角度(要らない気もする
#define ARM_MIN_DEG -110		//機構限界

enum ERROR{
	NONE = 0,
	SA_LOST = 0x01,
	SBUS_LOST = 0x02,
	Batt_UVL = 0x04,
};

class KawaRobo {
public:
	void setup(USART &serial,SBUS &sbus,SerialArduino &sa,NCP5359 &motor0,NCP5359 &motor1,NCP5359 &motor2,NCP5359 &motor3,GPIO &motorEnable);
	void uiSetup(Switch &sw0,Switch &sw1,LED &led0,LED &led1,LED &led2,LED &led3,LED &led4,LED &led5);
	void sensorSetup(ADC &adc0,ADC &adc1,ADC &adc2,ADC &adc3,float adcBatt);

	void cycle();
	void armPotCycle();
	void displayCycle();

	void run();

	void motorDisable();
	void motorEnable();

	float getRunPosition();
	float getArmPosition();
	float getRevPosition();
	float getSelectPosition();
	float getTogglePosition(uint16_t num);
	float getVolumePosition();
	int getStickPosition(uint16_t channel,uint16_t offset = 1024);

	void sbusDataUpdate();
	void saDataUpdate();

	float armDegree,pot1Int,pot2Int,armTargetDeg;
	int pot1Cnt,pot2Cnt,potCnt;

	int16_t dispValue;

	float adcToBattV,armOffsetDuty,battUnderVoltage;
	int32_t speakRequest,toggleStat[4],turnoverReturn;
	uint16_t serika,speakRequestNext;

	uint16_t mode,printValueSelect,selectToggle,error,oldError,saLedStat;
	uint64_t printTime,controlCycleIntervalTime,saSendTime,armSpeakTime;


private:
	USART *serial;
	NCP5359 *motor[4];
	GPIO *motorEn;
	SerialArduino *sa;
	SBUS *sbus;

	LED *led[6];
	Switch *sw[2];

	ADC *analog[4];

	TIM enc;
	AS504x mgEnc;

	PID robotR,robotTargetR,robotPIDR,armPID,armPitchGy,armPitch;
	PID armCurrent;
	Average<int> gravitatyAve;
	Average<float> battVoltage;
};


#endif
