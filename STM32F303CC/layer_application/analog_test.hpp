#ifndef ANALOG_TEST_HPP
#define ANALOG_TEST_HPP
#include "layer_application/console.hpp"
#include "layer_driver/board/pin.hpp"
#include "layer_driver/base/analog.hpp"
#include "mcutime.h"
#include <string.h>

/*�p�@
    1.TeraTerm��"analog"�Ƒł����ނ�pin�̏��Ƒ���L�[���o�Ă���B
	2.pin�̑���L�[�������ƑΉ�����pin�̃A�i���O��񂪏o�Ă���B
*/

/*
          �ҏW�ɂ��Ă͂�cpp��
*/

class AnalogPinKey{
public:
	char key;
	char pinName[10];
	Analog *analog;
	AnalogPinKey(){
		key=0;
	}
};

class AnalogTest:public ConsoleCommand{
public:
	AnalogTest(Console &console);
	int setup(){consolePrintf((char*)" analogIn Test->input 'analog'   and push 'ENTER'key\r\n");return 0;};
	int commandCompare(char *command){return ((!(strcmp(command,"a")))||(!(strcmp(command,"analog"))));};
	int commandCycle();
	int commandRequest(int argc,char **argv);
	void consoleReadChar(char data);
	void setPinStatus(Analog *ana,char key,char *pinName);
private:
	WatchDog dog;
	int pinValue;
	int pin;
	int64_t time;
	ConsoleCommand *command;
	AnalogPinKey anaPin[10];
};

#endif
