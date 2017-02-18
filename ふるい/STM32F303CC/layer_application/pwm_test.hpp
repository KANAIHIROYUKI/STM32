#ifndef PWM_TEST_HPP
#define PWM_TEST_HPP
#include "console.hpp"
#include "layer_driver/board/pin.hpp"
#include "layer_driver/base/pwm.hpp"
#include <string.h>
#include <stdlib.h>
#include "util.hpp"
#include "layer_driver/board/stm32f4_config/iwdg_software_reset.h"

/*�p�@
    1.TeraTerm��"pwm"�Ƒł����ނ�pin�̏��Ƒ���L�[���o�Ă���
	2.�g������TeraTerm��"pwm key duty" �Ɠ���ENTER�L�[�������ƁA
	     ����L�[�ɓK������pwm�s������duty�ɓ��ꂽ�o�͂��o�Ă���B
	key:TeraTerm��"pwm"����͂�ENTER�L�[�������Əo�Ă���pin�����Q��
   duty:0��duty��1�@�������g�p�\
*/

/*
          �ҏW�ɂ��Ă͂�cpp��
*/

class PwmPinKey{
public:
	char key;
	char pinName[10];
	Pwm *pwm;
	PwmPinKey(){
		key=0;
	}
};

class PwmTest:public ConsoleCommand{
public:
	PwmTest(Console &console);
	int setup(){consolePrintf((char*)"  p w m   Test->input  'pwm'     and push 'ENTER'key\r\n");return 0;};
	int commandCompare(char *command){return ((!(strcmp(command,"pwm"))));};
	int commandRequest(int argc,char **argv);
	void setPinStatus(Pwm *pwm,char key,char *pinName);
private:
	WatchDog dog;
	int pinValue;
	int pin;
	int pinData;
	ConsoleCommand *command;
	PwmPinKey pwmPin[20];
};

#endif // PWM_TEST_HPP_INCLUDED
