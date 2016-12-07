#ifndef CLOCK_TEST_HPP
#define CLOCK_TEST_HPP
#include "mcutime.h"
#include "console.hpp"
#include "layer_driver/board/pin.hpp"
#include "layer_driver/base/pwm.hpp"
#include <string.h>

class ClockTest:public ConsoleCommand{
public:
	ClockTest(Console &console);
	int setup(){consolePrintf((char*)"zClock Test->input 'millis'or'micros'   and push 'ENTER'key\r\n");return 0;};
	int commandCompare(char *command){return ((!(strcmp(command,"millis")))||(!(strcmp(command,"micros"))));};
	int commandCycle();
	int commandRequest(int argc,char **argv);
	void consoleReadChar(char data);
private:
	WatchDog dog;
	int mode;
	ConsoleCommand *command;
};

#endif // CLOCK_TEST_HPP
