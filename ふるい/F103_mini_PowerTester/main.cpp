#include "stm32f10x.h"
#include "stdio.h"
#include "SYSTEM.h"


#define IntervalTime (1000)*10
#define DriveTime 1000

uint64_t intervalTime = 0,iMin,iAve,iMax,vAve,vMin,vMax,inputV,inputI,senseCnt;

GPIO led;

GPIO usartTX;
GPIO usartRX;

GPIO vSensePin;
GPIO iSensePin;

USART serial;

ADC iSense;
ADC vSense;

int main(void)
{
	setup();

	iSensePin.setup(PA0,INPUT_AIN);
	vSensePin.setup(PA1,INPUT_AIN);

	led.setup(GPIOB,GPIO_Pin_0,GPIO_Mode_Out_PP);

	usartTX.setup(GPIOA,GPIO_Pin_9,GPIO_Mode_AF_PP);
	usartRX.setup(GPIOA,GPIO_Pin_10,GPIO_Mode_IN_FLOATING);

	iSense.setup(ADC1,0);
	vSense.setup(ADC1,1);


	serial.setup(USART1,921600);
	serial.printf("\n\rFILE = %s\n\r",__FILE__);
	serial.printf("DATE = %s\n\r",__DATE__);
	serial.printf("TIME = %s\n\r",__TIME__);

    while(1){
    	inputI = iSense.read();
    	inputV = vSense.read();
    	senseCnt++;

    	if(inputI < iMin)iMin = inputI;
    	if(inputV < vMin)vMin = inputV;
    	if(inputI > iMax)iMax = inputI;
    	if(inputV > vMax)vMax = inputV;

    	iAve += inputI;
    	vAve += inputV;

    	if(senseCnt > 1000){
    		intervalTime += IntervalTime;
    		delay(100);
    		led.toggle();
    		iAve = iAve / senseCnt;
    		vAve = vAve / senseCnt;

    		iMin *= 8.05;
    		iMax *= 8.05;
    		iAve *= 8.05;

    		vMax *= 8.05;
    		vMin *= 8.05;
    		vAve *= 8.05;

    		serial.printf("V-max = %4d,V-ave = %4d,V-min = %4d,I-max = %4d,I-ave = %4d,I-min = %4d\n\r",(uint32_t)vMax,(uint32_t)vAve,(uint32_t)vMin,(uint32_t)iMax,(uint32_t)iAve,(uint32_t)iMin);

    		iMin = 4098;
    		iMax = 0;
    		iAve = 0;

    		vMax = 0;
    		vMin = 4098;
    		vAve = 0;

    		senseCnt = 0;
    	}


    }
}
