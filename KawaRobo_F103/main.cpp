#include "main.hpp"

int main(void)
{
	setup();
    while(1){
    	sys.cycle();
    	kw.cycle();
    }
}
