#ifndef ENC_TEST_HPP_INCLUDED
#define ENC_TEST_HPP_INCLUDED

#include "console.hpp"
#include "layer_driver/board/pin.hpp"
#include "layer_driver/base/encoder.hpp"
#include "mcutime.h"
#include <string.h>

/*�p�@
    1.TeraTerm��"enc"�Ƒł����ނ�pin�̏��Ƒ���L�[���o�Ă���
	2.����L�[�������Ƃ��̃L�[�ɓK�������G���R�[�_�[�̏���ǂނ��Ƃ��ł���B
	(ESC�L�[�������ƃ}�C�R�����Z�b�g��������)
 */

/*
          �ҏW�ɂ��Ă͂�cpp��
 */

class EncKey{
public:
	char key;
	char pinName[10];
	Encoder *enc;
	EncKey(){
		key=0;
	}
};

class EncTest:public ConsoleCommand{
public:
	EncTest(Console &console);
	int setup(){consolePrintf((char*)" Encoder  Test->input  'enc'     and push 'ENTER'key\r\n");return 0;};
	int commandCompare(char *command){return (!(strcmp(command,"enc")));};
	int commandCycle();
	int commandRequest(int argc,char **argv);
	void consoleReadChar(char data);
	void setPinStatus(Encoder *enc,char key,char *pinName);
private:
	WatchDog dog;
	int pinValue;
	int pin;
	int64_t time;
	ConsoleCommand *command;
	EncKey encPin[20];
};

#endif // ENC_TEST_HPP_INCLUDED
