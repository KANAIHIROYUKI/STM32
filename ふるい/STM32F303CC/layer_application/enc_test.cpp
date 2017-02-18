#include "enc_test.hpp"

/*pwm_testの編集
	前提：pin.hppとpin.cppを基盤に合わせて編集を終わらせる。

	ピンを増やす場合：
					1.25行目付近にある　　setPinStatus(...);	をコピーして下に増やす。
					2.()の中に(new エンコーダ名,'key',"エンコーダ名")を書き込む。
						(keyはそのpinをキーボードで操作するときのキーとなるため半角英数とすること)
					3.終了

	ピンを減らす場合:
					1.25行目付近にある　　setPinStatus(...);のうち減らしたエンコーダと同じ名前のものが()内にあるものを消す。
					2.終了

	使えるピンはpin.hppで
					class エンコーダ名　のあとに　Encoderがついているものだけである。
					ex)class Enc0:public Encoder{...};
*/


EncTest::EncTest(Console &console):ConsoleCommand(console){
	pin=0;
	setPinStatus(new Enc0,'0',(char*)"Enc0");
	setPinStatus(new Enc1,'1',(char*)"Enc1");
	setPinStatus(new Enc2,'2',(char*)"Enc2");
	pinValue=-1;
	time=millis();
}

void EncTest::setPinStatus(Encoder *enc,char key,char *pinName){
	encPin[pin].enc=enc;
	encPin[pin].key=key;
	strcpy(encPin[pin].pinName,pinName);
	pin++;
}

int EncTest::commandRequest(int argc,char **argv){
	int i=0;
	for(i=0;i<pin;i++){
		encPin[i].enc->setup();
		consolePrintf((char*)"'%c'key:%s\r\n",encPin[i].key,encPin[i].pinName);
	}
	if(argc>=2){
		for(i=0;i<pin;i++){
			if(encPin[i].key==argv[1][0])
				pinValue=i;
		}
	}
	return 1;
}

void EncTest::consoleReadChar(char data){
	consolePrintf((char*)"\r\n");
	int i=0;
	pinValue=-1;
	if(data==0x1b)pinValue=-3;
	for(i=0;i<pin;i++){
		if(encPin[i].key==data)
			pinValue=i;
	}
}

int EncTest::commandCycle(){
	if(pinValue==-3){
		dog.setup();
	}
	if(millis()-time>=33){
		time=millis();
		if(pinValue>=0){
			consolePrintf((char*)"%s\t%d\r\n",encPin[pinValue].pinName,encPin[pinValue].enc->count());
		}
	}
	return 1;
}
