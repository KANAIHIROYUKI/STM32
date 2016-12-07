#ifndef PWM_TEST_HPP
#define PWM_TEST_HPP
#include "console.hpp"
#include "layer_driver/board/pin.hpp"
#include "layer_driver/base/pwm.hpp"
#include <string.h>
#include <stdlib.h>
#include "util.hpp"
#include "layer_driver/board/stm32f4_config/iwdg_software_reset.h"

/*用法
    1.TeraTermに"pwm"と打ち込むとpinの情報と操作キーが出てくる
	2.使い方はTeraTermに"pwm key duty" と入れENTERキーを押すと、
	     操作キーに適応したpwmピンからdutyに入れた出力が出てくる。
	key:TeraTermに"pwm"を入力しENTERキーを押すと出てくるpin情報を参照
   duty:0≦duty≦1　小数も使用可能
*/

/*
          編集についてははcppへ
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
