#include "clock_test.hpp"

ClockTest::ClockTest(Console &console):ConsoleCommand(console){mode=0;}

int ClockTest::commandRequest(int argc,char **argv){
		if(!(strcmp(argv[0],"millis")))
      mode=1;
    if(!(strcmp(argv[0],"micros")))
      mode=2;
	return 1;
}

int ClockTest::commandCycle(){
  if(mode==1)
    consolePrintf((char*)"millis=%d\r\n",millis());
  if(mode==2)
    consolePrintf((char*)"micros=%d\r\n",micros());
  return 1;
};

void ClockTest::consoleReadChar(char data){
	if(data==0x1b)
		dog.setup();
}
