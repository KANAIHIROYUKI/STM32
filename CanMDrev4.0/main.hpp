#ifndef MAIN_H_
#define MAIN_H_

#include "system.h"
#include "app.h"

#include "stdio.h"
#include "stdint.h"
#include "math.h"

#define BUZZER_DO 2093
#define BUZZER_RE 2302
#define BUZZER_MI 2533
#define BUZZER_FA 2786
#define BUZZER_SOL 3064
#define BUZZER_RA 3371
#define BUZZER_SI 3708

#define ENC_CPR 2000
#define MOTOR_RESISTANCE 0.117
#define MOTOR_REVOLUTION_PER_VOLTAGE 9.68

#define POWER_SUPPY_VOLTAGE 16.0


TIM pwm[4],enPwm[2],enc[2];
CAN can1;
USART serial,vserial;
ADC cs[2];
GPIO add[4],sw[2],led[2],io[8],en[2];

MOTOR motor[2];
CanNodeMotorDriver nodeMotor[2];
CanNodeValve nodeValve;

CanMotorDriver canMD[2];
CanValve canVLV;


#define DEBUG

#define TIM_CNT 1350
//#define TIM_CNT 2048
//PWMé¸ä˙ÇåàíËÇ∑ÇÈ°ADCÇÃïœä∑éûä‘Ç…àÍâûçáÇÌÇπÇƒÇ†ÇÈ


#define CAN_ADD 15 - (add[0].read()*2) - add[1].read() - (add[2].read()*8) - (add[3].read()*4)

uint16_t rxFlag = 0;
uint8_t canData[8] = {0,0,0,0,0,0,0,0},canAddress=0;
CanRxMsg rxMessage;

#define PRINT_TIME 50

#define CYCLE_TIME 10

uint64_t printTime = 0,cycleTime = 0;

int64_t currentInt[2] = {0,0},currentCnt[2] = {0,0},currentOffset[2] = {0,0};

void motorBuzzer1(uint32_t frequency,float volume);
void motorBuzzer2(uint32_t frequency,float volume);
void motorBuzzerEnd();

void setup(){
	systemSetup();

	vserial.setup(USART1,9600,PA9,PA10);

	io[0].setup(PA3,OUTPUT);
	io[1].setup(PA4,OUTPUT);
	io[2].setup(PA5,OUTPUT);
	io[3].setup(PB2,OUTPUT);
	io[4].setup(PB12,OUTPUT);
	io[5].setup(PB13,OUTPUT);
	io[6].setup(PB14,OUTPUT);
	io[7].setup(PB15,OUTPUT);
/*
	while(1){
		delay(200);
		for(int i=0;i<8;i++){
			led[i].toggle();
		}
	}*/


	enc[0].encoderSetup(TIM2,PA0,PA1);
	enc[1].encoderSetup(TIM3,PA6,PA7);
/*
	motorBuzzer1(BUZZER_DO,0.04);
	delay(50);
	motorBuzzer1(BUZZER_DO,0);

	delay(100);

	motorBuzzer1(BUZZER_MI,0.05);
	delay(100);
	motorBuzzer1(BUZZER_MI,0);


	motorBuzzer2(BUZZER_DO,0.04);
	delay(100);
	motorBuzzer2(BUZZER_DO,0);

	delay(50);

	motorBuzzer2(BUZZER_MI,0.05);
	delay(100);
	motorBuzzer2(BUZZER_MI,0);

*/
	pwm[0].pwmSetup(TIM4,1,PB6,TIM_CNT);
	pwm[1].pwmSetup(TIM4,2,PB7,TIM_CNT);
	pwm[2].pwmSetup(TIM4,3,PB8,TIM_CNT);
	pwm[3].pwmSetup(TIM4,4,PB9,TIM_CNT);
	enPwm[0].pwmSetup(TIM1,1,PA8,TIM_CNT);
	enPwm[1].pwmSetup(TIM1,2,PA9,TIM_CNT);

	enPwm[0].duty(TIM_CNT);
	enPwm[1].duty(TIM_CNT);

	pwm[0].duty(TIM_CNT/2);
	pwm[1].duty(TIM_CNT/2);
	pwm[2].duty(TIM_CNT/2);
	pwm[3].duty(TIM_CNT/2);

	add[0].setup(PC13,INPUT_PU);
	add[1].setup(PC14,INPUT_PU);
	add[2].setup(PC15,INPUT_PU);
	add[3].setup(PB2,INPUT_PU);

	sw[0].setup(PB4,INPUT_PU);
	sw[1].setup(PB5,INPUT_PU);
	led[0].setup(PA15,OUTPUT);
	led[1].setup(PB3,OUTPUT);

	led[0].write(LOW);
	led[1].write(LOW);

	motor[0].setup(pwm[0],pwm[1]);
	motor[1].setup(pwm[2],pwm[3]);
	motor[0].enPwmAssigne(enPwm[0]);
	motor[1].enPwmAssigne(enPwm[1]);

	cs[0].setup(ADC1,8,PB0);
	cs[1].setup(ADC2,9,PB1);

	can1.setup(CAN1,PA12,PA11);

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);

	nodeMotor[0].setup(motor[0],can1,0);
	nodeMotor[1].setup(motor[1],can1,1);

	nodeMotor[0].ledAssign(led[0]);
	nodeMotor[1].ledAssign(led[1]);

	nodeValve.setup(can1,0);

	canMD[0].setup(can1,0);
	canMD[1].setup(can1,1);

	canVLV.setup(can1,0);

	serial.setup(USART3,921600,PB10,PB11);
	serial.printf("\n\rFILE = %s\n\rDATE = %s\n\rTIME = %s\n\rADD = %d\n\r",__FILE__,__DATE__,__TIME__,CAN_ADD);

}

extern "C" void USB_LP_CAN1_RX0_IRQHandler(void){
	rxFlag++;
	can1.receive();
	nodeMotor[0].interrupt();
	nodeMotor[1].interrupt();
	return;
}


void motorBuzzerEnd(){
	pwm[0].pwmSetup(TIM4,1,PB6,TIM_CNT);
	pwm[1].pwmSetup(TIM4,2,PB7,TIM_CNT);
	pwm[2].pwmSetup(TIM4,3,PB8,TIM_CNT);
	pwm[3].pwmSetup(TIM4,4,PB9,TIM_CNT);
	enPwm[0].pwmSetup(TIM1,1,PA8,4000);
	enPwm[1].pwmSetup(TIM1,2,PA9,4000);
	enPwm[0].duty(TIM_CNT);
	enPwm[1].duty(TIM_CNT);

	pwm[0].duty(TIM_CNT);
	pwm[1].duty(TIM_CNT);
	pwm[2].duty(TIM_CNT);
	pwm[3].duty(TIM_CNT);
}

void motorBuzzer1(uint32_t frequency,float volume){
	frequency = 72000000/frequency;
	pwm[0].pwmSetup(TIM4,1,PB6,frequency,0,TIM_OCMode_PWM1);
	pwm[1].pwmSetup(TIM4,2,PB7,frequency,0,TIM_OCMode_PWM2);
	enPwm[0].pwmSetup(TIM1,1,PA8,frequency/20);

	pwm[0].duty(frequency/2);
	pwm[1].duty(frequency/2);
	enPwm[0].duty(frequency*volume);
}

void motorBuzzer2(uint32_t frequency,float volume){
	frequency = 72000000/frequency;
	pwm[2].pwmSetup(TIM4,3,PB8,frequency,0,TIM_OCMode_PWM1);
	pwm[3].pwmSetup(TIM4,4,PB9,frequency,0,TIM_OCMode_PWM2);
	enPwm[1].pwmSetup(TIM1,2,PA9,frequency/20);

	pwm[2].duty(frequency/2);
	pwm[3].duty(frequency/2);
	enPwm[1].duty(frequency*volume);
}

#endif
