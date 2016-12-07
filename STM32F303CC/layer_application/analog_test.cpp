#include "analog_test.hpp"

/*analog_test�̕ҏW
	�O��Fpin.hpp��pin.cpp����Ղɍ��킹�ĕҏW���I��点��B

	�s���𑝂₷�ꍇ�F
					1.25�s�ڕt�߂ɂ���@�@setPinStatus(...);	���R�s�[���ĉ��ɑ��₷�B
					2.()�̒���(new �s����,'key',"�s����")���������ށB
						(key�͂���pin���L�[�{�[�h�ő��삷��Ƃ��̃L�[�ƂȂ邽�ߔ��p�p���Ƃ��邱��)
					3.�I��
	�s�������炷�ꍇ:
					1.25�s�ڕt�߂ɂ���@�@setPinStatus(...);�̂������炵��pin�Ɠ������O�̂��̂�()���ɂ�����̂������B
					2.�I��
	�g����s����pin.hpp��
						class �s�����@�̂��ƂɁ@Analog�����Ă�����̂����ł���B
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
