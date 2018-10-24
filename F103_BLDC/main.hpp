#ifndef MAIN_H_
#define MAIN_H_

#include "system.h"
#include "app.h"
#include "util.h"

#define State0 PB6
#define State1 PB7
#define State2 PB8
#define Dowel  PB5

System sys;

USART serial;
uint64_t intervalTime,speedCycleTome;

GPIO ss,gdNss;
SPI_Master spi;
AS504x mag;
TIM enc;

LED led[2];

GPIO pwGd,gdEn,gdWake,nFault;
ADC cs[3],vs[3],vBatt;

GPIO state[6],dwell,pwmIO;
TIM pwm;

DRV8305 drv;

uint8_t data[16];
uint16_t pwmDuty = 50;
int32_t val;
int64_t encVal;

uint16_t isCW;
int64_t oldCnt,speed;
int offsetCnt = 0,tim3_cc1_cnt;

void setup(){
	sys.setup();
	serial.setup(USART1_Setup,115200);
	sys.usartSetup(serial);

	spi.setup(SPI2_Setup);
	ss.setup(PB12,OUTPUT);
	gdNss.setup(PA8,OUTPUT);
	ss.write(1);
	gdNss.write(1);

	enc.encoderSetup(TIM3_ENC_Setup);
	enc.itSetup(TIM_IT_CC1);
	enc.itSetup(TIM_IT_CC2);
	TIM_SetCompare1(TIM3,100);

	mag.setup(spi,ss);

	led[0].setup(PC15);
	led[1].setup(PC14);

	led[0].interval(500);
	led[1].interval(500);

	pwm.pwmSetup(TIM4,4,PB9,500);

	state[0].setup(PB6,OUTPUT);
	state[1].setup(PB7,OUTPUT);
	state[2].setup(PB8,OUTPUT);
	dwell.setup(PB5,OUTPUT);

	cs[0].setup(PA0ADC1_Setup);
	cs[1].setup(PA1ADC1_Setup);
	cs[2].setup(PA2ADC1_Setup);

	vs[0].setup(PA3ADC2_Setup);
	vs[1].setup(PA4ADC2_Setup);
	vs[2].setup(PA5ADC2_Setup);

	vBatt.setup(PB0ADC2_Setup);

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);

	pwGd.setup(PB1,INPUT_PU);
	gdEn.setup(PB2,OUTPUT);
	gdWake.setup(PB10,OUTPUT);
	nFault.setup(PB11,INPUT_PU);

	gdEn.write(1);
	gdWake.write(1);

	drv.setup(spi,gdNss);
	delay(1);
	drv.programing();
}


void TIM3_PWM_CC1_IRQ(){
	tim3_cc1_cnt++;
}


#endif
