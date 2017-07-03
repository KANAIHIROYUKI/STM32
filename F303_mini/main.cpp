#include "gpio.h"
#include "adc.h"
#include "usart.h"
#include "systick.h"
#include "util.h"

int main(void)
{
	SystickSetup();
	GPIOSetup();

	pinSetup(PB0,OUTPUT);
	pinSetup(PA9,OUTPUT_AF);
	pinSetup(PA10,GPIO_Mode_IN);



    while(1)
    {

    	delay(100);
    	pinToggle(PB0);
    }
}
