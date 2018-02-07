#include "main.hpp"

int main(void)
{
	setup();
	motorEn.write(1);

    while(1){
    	sys.cycle();
    	kw.cycle();
    }
}
