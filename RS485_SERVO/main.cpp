#include "main.hpp"
#include "util.hpp"

int main(void)
{
	setup();
	led[0].interval(100);

    while(1){
    	int dir = 1,gpos = 0;

    	for(int i=0;i<4;i++){
    		led[i].cycle();
    	}

    	if(millis() > intervalTime){
    		intervalTime += IntervalTime;
    		//servo(0,512);

    		sr518.WriteWordData(1,30,gpos,NULL);
    		delayMicros(100);
    		sr518.WriteWordData(1,30,gpos,NULL);
    		  gpos += dir;
    		  if (gpos >= 300) {
    		    gpos = 300;
    		    dir *= -1;
    		  } else if (gpos <= 90) {
    		    gpos = 90;
    		    dir *= -1;
    		  };

    	}
    }
}
