#include "analog_test.hpp"

/*analog_testの編集
	前提：pin.hppとpin.cppを基盤に合わせて編集を終わらせる。

	ピンを増やす場合：
					1.25行目付近にある　　setPinStatus(...);	をコピーして下に増やす。
					2.()の中に(new ピン名,'key',"ピン名")を書き込む。
						(keyはそのpinをキーボードで操作するときのキーとなるため半角英数とすること)
					3.終了
	ピンを減らす場合:
					1.25行目付近にある　　setPinStatus(...);のうち減らしたpinと同じ名前のものが()内にあるものを消す。
					2.終了
	使えるピンはpin.hppで
						class ピン名　のあとに　Analogがついているものだけである。
						ex)class A0:public Analog{...};
*/

AnalogTest::AnalogTest(Console &console):ConsoleCommand(console){
	pin=0;
	setPinStatus(new A0,'0',(char*)"A0");
	setPinStatus(new A1,'1',(char*)"A1");
	setPinStatus(new A2,'2',(char*)"A2");
	setPinStatus(new A3,'3',(char*)"A3");
	setPinStatus(new A4,'4',(char*)"A4");
	setPinStatus(new A5,'5',(char*)"A5");
	setPinStatus(new A6,'6',(char*)"A6");
	setPinStatus(new A7,'7',(char*)"A7");
	pinValue=-1;
	time=millis();
}

void AnalogTest::setPinStatus(Analog *ana,char key,char *pinName){
	anaPin[pin].analog=ana;
	anaPin[pin].key=key;
	strcpy(anaPin[pin].pinName,pinName);
	pin++;
}

int AnalogTest::commandRequest(int argc,char **argv){
	int i;
	for(i=0;i<pin;i++)
		consolePrintf((char*)"'%c'key:%s\r\n",anaPin[i].key,anaPin[i].pinName);
	consolePrintf((char*)"AnalogIn Mode\r\n");
	return 1;
}

void AnalogTest::consoleReadChar(char data){
	consolePrintf((char*)"\r\n");
	int i=0;
	pinValue=-1;
	if(data=='@')      pinValue=-2;
	else if(data==0x1b)pinValue=-3;
	for(i=0;i<pin;i++){
		if(anaPin[i].key==data)
			pinValue=i;
	}
		if(pinValue>=0){
			consolePrintf((char*)"pin:%s\t%f\r\n",anaPin[pinValue].pinName,anaPin[pinValue].analog->analogRead());
		}
}

int AnalogTest::commandCycle(){
	if(pinValue==-3){
		dog.setup();
	}
	if(millis()-time>=33){
		time=millis();
			if(pinValue>=0){
				consolePrintf((char*)"pin:%s\t%f\r\n",anaPin[pinValue].pinName,anaPin[pinValue].analog->analogRead());
			}
	}
	return 1;
}
