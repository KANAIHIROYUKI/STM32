#include "main.hpp"
#include "util.hpp"

int main(void)
{
	setup();
    while(1){

    	kw.cycle();
    	if(millis() > intervalTime){
    		intervalTime += IntervalTime;

    	}
    }
}
