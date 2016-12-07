#include "digital_test.hpp"
#include "layer_driver/board/board.h"

/*digital_test�̕ҏW
	�O��Fpin.hpp��pin.cpp����Ղɍ��킹�ĕҏW���I��点��B

	�s���𑝂₷�ꍇ�F
					1.23�s�ڕt�߂ɂ���@�@setPinStatus(...);	���R�s�[���ĉ��ɑ��₷�B
					2.()�̒���(new �s����,'key',"�s����")���������ށB
						(key�͂���pin���L�[�{�[�h�ő��삷��Ƃ��̃L�[�ƂȂ邽�ߔ��p�p��"1����"�Ƃ��邱��)
						(key��DigitalTest���œ������͎̂g��Ȃ�����)
					3.�I��
	�s�������炷�ꍇ:
					1.23�s�ڕt�߂ɂ���@�@setPinStatus(...);�̂������炵��pin�Ɠ������O�̂��̂�()���ɂ�����̂������B
					2.�I��
	�g����s����pin.hpp��
					class �s�����@�̂��ƂɁ@Digital�����Ă�����̂����ł���B
					ex)class A0:public Digital{...};
*/

DigitalTest::DigitalTest(Console &console):ConsoleCommand(console){
	pin=0;
	setPinStatus(new D0,'0',(char*)"D0");
	setPinStatus(new D1,'1',(char*)"D1");
	setPinStatus(new D2,'2',(char*)"D2");
	setPinStatus(new D3,'3',(char*)"D3");
	setPinStatus(new A0,'4',(char*)"A0");
	setPinStatus(new A1,'5',(char*)"A1");
	setPinStatus(new A2,'6',(char*)"A2");
	setPinStatus(new A3,'7',(char*)"A3");
	setPinStatus(new A4,'8',(char*)"A4");
	setPinStatus(new A5,'9',(char*)"A5");
	setPinStatus(new A6,'q',(char*)"A6");
	setPinStatus(new A7,'w',(char*)"A7");
	setPinStatus(new Pwm0,'e',(char*)"Pwm0");
	setPinStatus(new Pwm1,'r',(char*)"Pwm1");
	setPinStatus(new Pwm2,'t',(char*)"Pwm2");
	setPinStatus(new Pwm3,'y',(char*)"Pwm3");
	setPinStatus(new Pwm4,'u',(char*)"Pwm4");

	in=false;
	pinValue=-1;
	time=millis();
}

void DigitalTest::setPinStatus(Digital *dig,char key,char *pinName){
	digPin[pin].dig=dig;
	digPin[pin].key=key;
	strcpy(digPin[pin].pinName,pinName);
	pin++;
}

int DigitalTest::commandRequest(int argc,char **argv){
	int i=0;
	for(i=0;i<pin;i++){
		consolePrintf((char*)"'%c'key:%s\r\n",digPin[i].key,digPin[i].pinName);
	}
	if(argc>=2){
		if(!(strcmp(argv[1],"i"))){
			consolePrintf((char*)"digitalIn Mode\r\n");
			for(i=0;i<pin;i++){
				digPin[i].dig->setupDigitalInPullDown();
			}
			in=true;
			return 1;
		}
		if(!(strcmp(argv[1],"o"))){
			consolePrintf((char*)"digitalOut Mode\r\n");
			for(i=0;i<pin;i++){
				digPin[i].dig->setupDigitalOut();
			}
			in=false;
			return 1;
		}
	}
	return 0;
}

void DigitalTest::consoleReadChar(char data){
	consolePrintf((char*)"\r\n");
	int i=0;
	pinValue=-1;
	if(data=='@')      pinValue=-2;
	else if(data==0x1b)pinValue=-3;
	for(i=0;i<pin;i++){
		if(digPin[i].key==data)
			pinValue=i;
	}
	if(in==false){
		if(pinValue>=0){
			digPin[pinValue].dig->digitalToggle();
			consolePrintf((char*)"pin:%s\t%d\r\n",digPin[pinValue].pinName,digPin[pinValue].dig->digitalRead());
		}
		if(pinValue==-2){
			for(i=0;i<pin;i++){
				digPin[i].dig->digitalLow();
			}
		}
	}
	else if(in==true){
		if(pinValue>=0){
			consolePrintf((char*)"pin:%s\t%d\r\n",digPin[pinValue].pinName,digPin[pinValue].dig->digitalRead());
		}
	}
}

int DigitalTest::commandCycle(){
	if(pinValue==-3){
		dog.setup();
	}
	if(millis()-time>=33){
		time=millis();
		if(in==true){
			if(pinValue>=0){
				consolePrintf((char*)"pin:%s\t%d\r\n",digPin[pinValue].pinName,digPin[pinValue].dig->digitalRead());
			}
		}
	}
	return 1;
}
