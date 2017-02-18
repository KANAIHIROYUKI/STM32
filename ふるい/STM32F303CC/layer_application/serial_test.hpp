#ifndef SERIAL_TEST_HPP
#define SERIAL_TEST_HPP
#include "layer_application/console.hpp"
#include "layer_driver/board/pin.hpp"
#include "layer_driver/base/digital.hpp"
#include "mcutime.h"
#include <string.h>

class SerialTest{
public:
	SerialTest();
	int commandCompare(char *command){return ((!(strcmp(command,"s")))||(!(strcmp(command,"serial"))));};
private:
	Console console[5];
};


#endif
