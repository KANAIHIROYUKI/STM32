#include "main.hpp"

int main(void)
{
	setup();
	motorEn.write(1);

    while(1){
    	spiSS.toggle();
    	sys.cycle();
    	kw.cycle();
    }
}
