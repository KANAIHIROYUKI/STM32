#ifndef MAIN_H_
#define MAIN_H_

#include "system.h"
#include "app.h"
#include "util.h"
System sys;

USART serial;
uint64_t intervalTime;

GPIO ss,gdNss;
SPI_Master spi;
TIM enc;

LED led[2];

GPIO pwGd,gdEn,gdWake,nFault;
ADC cs[3],vs[3],vBatt;

GPIO state[3],dwell;
TIM pwm;

DRV8305 drv;

uint8_t data[16];

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

	led[0].setup(PC15);
	led[1].setup(PC14);

	led[0].interval(500);
	led[1].interval(500);

	pwm.pwmSetup(TIM4_PWM4_Setup,2000);
	state[0].setup(PB6,OUTPUT);
	state[1].setup(PB7,OUTPUT);
	state[2].setup(PB8,OUTPUT);
	dwell.setup(PB5,OUTPUT);

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



#endif
