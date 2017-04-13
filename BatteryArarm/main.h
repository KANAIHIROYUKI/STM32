#include "stm32f10x.h"
#include "stdio.h"
#include "system.h"
#include "app.h"



#define IntervalTime 50
#define BUZZER_FRQ 2000

#define CELL0_VOLTAGE_GAIN 8.86
#define CELL1_VOLTAGE_GAIN 8.86
#define CELL2_VOLTAGE_GAIN 8.86

#define CELL3_VOLTAGE_GAIN 8.86
#define CELL4_VOLTAGE_GAIN 8.86
#define CELL5_VOLTAGE_GAIN 8.86

uint64_t intervalTime = 0,voltage[6],vmin[6],vave[6],vmax[6],vcnt=0;
uint32_t frequency,buzzerPower,buzzerStat,buzzerStatCnt,cellMax,cellMin,cellWorst,beepInterval;


GPIO power,signal;
TIM buzzer;
USART serial,hc04;
ADC batt[6];


void beep(uint32_t frq,float buzzer_duty);

void setup(){
	systemSetup();

	batt[0].setup(ADC1,9,PB1);
	batt[1].setup(ADC1,8,PB0);
	batt[2].setup(ADC1,7,PA7);
	batt[3].setup(ADC1,6,PA6);
	batt[4].setup(ADC1,5,PA5);
	batt[5].setup(ADC1,4,PA4);

	power.setup(PA1,OUTPUT);
	power.write(1);

	signal.setup(PB2,OUTPUT);
	signal.write(1);

	frequency = 72000000/BUZZER_FRQ;
	frequency = 72000;
	buzzer.pwmSetup(TIM2,1,PA0,frequency);
	buzzer.duty(frequency/2);

	hc04.setup(USART2,115200,PA2,PA3);
	hc04.printf("hogehoge\n\r");
	serial.setup(USART1,921600,PA9,PA10);
	serial.printf("\n\rFILE = %s\n\r",__FILE__);
	serial.printf("DATE = %s\n\r",__DATE__);
	serial.printf("TIME = %s\n\r",__TIME__);

	delay(100);
	signal.write(0);
	power.write(0);
	buzzer.duty(0);
}

extern "C" void USB_LP_CAN1_RX0_IRQHandler(void){
	return;
}

void beep(uint32_t frq,float buzzer_duty){
	frequency = 72000000/frq;
	buzzer.pwmSetup(TIM2,1,PA0,frequency);
	buzzer.dutyF(buzzer_duty);
}
