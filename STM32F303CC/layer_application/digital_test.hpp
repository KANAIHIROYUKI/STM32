#ifndef DIGITAL_TEST_HPP
#define DIGITAL_TEST_HPP
#include "console.hpp"
#include "layer_driver/board/pin.hpp"
#include "layer_driver/base/digital.hpp"
#include "mcutime.h"
#include <string.h>

/*
�`��(ry
DigitalTest�̎g����

�f�W�^���̏o�͂����������Ƃ���	Tera Term�̃R���\�[���� "digital o"�Ɠ��͂���ENTER�L�[�������܂��B
�f�W�^���̓��͂����������Ƃ���	Tera Term�̃R���\�[���� "digital i"�Ɠ��͂���ENTER�L�[�������܂��B
�@(Tera Term �ɓ��͂ł��Ȃ��ꍇ��Tx.Rx�̐����������ڑ�����ĂȂ��\��������܂��B(�t�ł���\���������ł�))

��L��code�𐳂������͂���Ǝg�p�ł���pin��pin�ɑΉ������L�[���o�Ă��܂��B
�@Test������key��Tera Term �ɓ��͂���Ƃ��ꂼ��Ή�����pin��
     �o��mode�̎��̓L�[���������тɏo�͂�HIGH��LOW�𔽓]��
     ����mode�̎���'pulldown'�Ƃ��č�p���l��Ԃ��܂��B

     ���o�͂�mode��؂�ւ���Ƃ���'ESC'�L�[���������ƂŃ��Z�b�g����A�Ă�mode��I�����܂��B
 */

/*
�ҏW�ɂ��Ă͂�cpp��
 */

class DigitalPinKey{
public:
	char key;
	char pinName[10];
	Digital *dig;
	DigitalPinKey(){
		key=0;
	}
};

class DigitalTest:public ConsoleCommand{
public:
	DigitalTest(Console &console);
	int setup(){
		consolePrintf((char*)"digitalOutTest->input'digital o' and push 'ENTER'key\r\n");
		consolePrintf((char*)"digitalIn Test->input'digital i' and push 'ENTER'key\r\n");
		return 0;
	};
	int commandCompare(char *command){return ((!(strcmp(command,"d")))||(!(strcmp(command,"digital"))));};
	int commandCycle();
	int commandRequest(int argc,char **argv);
	void consoleReadChar(char data);
	void setPinStatus(Digital *dig,char key,char *pinName);
private:
	bool in;
	WatchDog dog;
	int pinValue;
	int pin;
	int64_t time;
	ConsoleCommand *command;
	DigitalPinKey digPin[20];
};

#endif // CIRCUIT_TEST_HPP_INCLUDED
