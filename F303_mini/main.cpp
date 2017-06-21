#include "gpio.h"
#include "systick.h"

int main(void)
{
	SystickSetup();
	GPIOSetup();

	pinSetup(PB0,OUTPUT);
	pinWrite(PB0,Bit_SET);

    while(1)
    {
    	//pinToggle(PB0);

    	pinWrite(PB0,Bit_SET);
    	delay(500);
    	pinWrite(PB0,Bit_RESET);
    	delay(500);
    }
}
