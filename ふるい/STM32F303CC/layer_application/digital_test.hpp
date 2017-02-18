#ifndef DIGITAL_TEST_HPP
#define DIGITAL_TEST_HPP
#include "console.hpp"
#include "layer_driver/board/pin.hpp"
#include "layer_driver/base/digital.hpp"
#include "mcutime.h"
#include <string.h>

/*
チン(ry
DigitalTestの使い方

デジタルの出力を試したいときは	Tera Termのコンソールに "digital o"と入力してENTERキーを押します。
デジタルの入力を試したいときは	Tera Termのコンソールに "digital i"と入力してENTERキーを押します。
　(Tera Term に入力できない場合はTx.Rxの線が正しく接続されてない可能性があります。(逆である可能性が高いです))

上記のcodeを正しく入力すると使用できるpinとpinに対応したキーが出てきます。
　TestしたいkeyをTera Term に入力するとそれぞれ対応したpinが
     出力modeの時はキーを押すたびに出力をHIGHとLOWを反転し
     入力modeの時は'pulldown'として作用し値を返します。

     入出力のmodeを切り替えるときは'ESC'キーを押すことでリセットされ、再びmodeを選択します。
 */

/*
編集についてははcppへ
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
