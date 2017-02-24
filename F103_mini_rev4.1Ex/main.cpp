#include "main.h"

int main(void)
{
	setup();

	while(1){
		serial.printf("vbatt = %4d,v1a = %4d,bab = %4d,v2a = %4d,v2b = %4d\n\r",vbatt.read(),v1a.read(),v1b.read(),v2a.read(),v2b.read());
		delay(100);
	}
}
