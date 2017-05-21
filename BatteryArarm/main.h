#include "stm32f10x.h"
#include "stdio.h"
#include "system.h"
#include "app.h"



#define IntervalTime 10
#define BUZZER_FRQ 2000

#define BoardNumber1


#ifdef BoardNumber1
#define CELL0_VOLTAGE_GAIN 4.4693
#define CELL1_VOLTAGE_GAIN 4.5130
#define CELL2_VOLTAGE_GAIN 4.5096

#define CELL3_VOLTAGE_GAIN 8.7082
#define CELL4_VOLTAGE_GAIN 8.6445
#define CELL5_VOLTAGE_GAIN 8.7272
#endif



#ifdef BoardNumber2
#define CELL0_VOLTAGE_GAIN 4.5062
#define CELL1_VOLTAGE_GAIN 4.6048
#define CELL2_VOLTAGE_GAIN 4.5697

#define CELL3_VOLTAGE_GAIN 8.6643
#define CELL4_VOLTAGE_GAIN 8.7082
#define CELL5_VOLTAGE_GAIN 8.7209
#endif

#ifdef BoardNumber3
#define CELL0_VOLTAGE_GAIN 4.5836
#define CELL1_VOLTAGE_GAIN 4.6261
#define CELL2_VOLTAGE_GAIN 4.6545

#define CELL3_VOLTAGE_GAIN 8.7463
#define CELL4_VOLTAGE_GAIN 8.9888
#define CELL5_VOLTAGE_GAIN 8.7977
#endif


enum {
	Batt_Setup,
	Batt_Norml,
	Batt_Error,
};

uint64_t ledInterval,intervalTime = 0,battv,voltage[6],vmin[6],vave[6],vmax[6],vcnt=0,cellNum;
uint32_t frequency,buzzerPower,buzzerStatCnt,cellMax,cellMin,cellWorst,beepEnable;
int32_t beepInterval;


GPIO power;
TIM buzzer;
USART serial;
ADC batt[6];


void beep(uint32_t frq,float buzzer_duty);

void setup(){
	systemSetup();

	batt[5].setup(ADC1,9,PB1);
	batt[4].setup(ADC1,8,PB0);
	batt[3].setup(ADC1,7,PA7);
	batt[2].setup(ADC1,6,PA6);
	batt[1].setup(ADC1,5,PA5);
	batt[0].setup(ADC1,4,PA4);

	power.setup(PA1,OUTPUT);
	power.write(1);

	buzzer.pwmSetup(TIM2,1,PA0,72000);

	serial.setup(USART1,921600,PA9,PA10);
	serial.printf("\n\rFILE = %s\n\r",__FILE__);
	serial.printf("DATE = %s\n\r",__DATE__);
	serial.printf("TIME = %s\n\r",__TIME__);

	delay(100);

	power.write(0);
	buzzer.duty(0);

	delay(100);

	voltage[0] = (batt[0].read()*CELL0_VOLTAGE_GAIN);
	voltage[1] = (batt[1].read()*CELL1_VOLTAGE_GAIN);
	voltage[2] = (batt[2].read()*CELL2_VOLTAGE_GAIN);
	voltage[3] = (batt[3].read()*CELL3_VOLTAGE_GAIN);
	voltage[4] = (batt[4].read()*CELL4_VOLTAGE_GAIN);
	voltage[5] = (batt[5].read()*CELL5_VOLTAGE_GAIN);

	for(int i=0;i<6;i++){
		if(voltage[i] > 3000)cellNum = i;		//ÉZÉãêîåüèo
	}
	serial.printf("cell number = %d\n\r",cellNum+1);

	for(int i=0;i<(cellNum+1);i++){
		beep(4000,0.5);
		delay(100);
		beep(4000,0);
		delay(10);
	}

	ledInterval = millis();
}

extern "C" void USB_LP_CAN1_RX0_IRQHandler(void){
	return;
}

void beep(uint32_t frq,float buzzer_duty){
	frequency = 72000000/frq;
	buzzer.pwmSetup(TIM2,1,PA0,frequency);
	buzzer.dutyF(buzzer_duty);
}
