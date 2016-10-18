#include "stm32f10x.h"
#include "stdio.h"


#include "SYSTEM.h"


#define CAN_ID 0x280

#define IntervalTime 100

uint16_t a,adcChannelStat = 1;
uint16_t rxFlag = 0,tim4itCnt=0,currentA_16,currentB_16;

float currentA_A=0,currentB_A=0;

uint8_t canData[8] = {0,0,0,0,0,0,0,0},canAddress=0;

uint64_t intervalTimer = 0;


GPIO encA;
GPIO encB;

GPIO led;

GPIO usartTX;
GPIO usartRX;

GPIO pinPwm;
GPIO pinA;
GPIO pinB;

TIM pwm;
TIM enc;

USART serial;

void motor(int duty);

int main(void)
{
	setup();

	encA.setup(GPIOA,GPIO_Pin_6,GPIO_Mode_IPU);
	encB.setup(GPIOA,GPIO_Pin_7,GPIO_Mode_IPU);
	enc.encoderSetup(TIM3);

	led.setup(GPIOB,GPIO_Pin_0,GPIO_Mode_Out_PP);

	usartTX.setup(GPIOA,GPIO_Pin_9,GPIO_Mode_AF_PP);
	usartTX.setup(GPIOA,GPIO_Pin_10,GPIO_Mode_IN_FLOATING);

	pinPwm.setup(GPIOA,GPIO_Pin_0,GPIO_Mode_AF_PP);
	pinA.setup(GPIOA,GPIO_Pin_1,GPIO_Mode_Out_PP);
	pinB.setup(GPIOA,GPIO_Pin_2,GPIO_Mode_Out_PP);
	pinA.write(Bit_RESET);
	pinB.write(Bit_RESET);

	pwm.pwmSetup(TIM2,1,1024,TIM_OCMode_PWM1);
	pwm.duty(0);


	serial.setup(USART1,921600);
	serial.printf("FILE = %s\n\r",__FILE__);
	serial.printf("DATE = %s\n\r",__DATE__);
	serial.printf("TIME = %s\n\r",__TIME__);


    while(1){
    	serial.printf("%d\n\r",millis());

    	delay(100);
    	led.toggle();


    }
}

void motor(int duty){
	if(duty == 0){
		pinA.write(Bit_RESET);
		pinB.write(Bit_RESET);
		pwm.duty(0);
	}else if(duty > 0){
		pinA.write(Bit_RESET);
		pinB.write(Bit_SET);
		pwm.duty(duty);
	}else{
		pinA.write(Bit_SET);
		pinB.write(Bit_RESET);
		pwm.duty(-duty);
	}
}

