#include "enc_test.hpp"

/*pwm_test�̕ҏW
	�O��Fpin.hpp��pin.cpp����Ղɍ��킹�ĕҏW���I��点��B

	�s���𑝂₷�ꍇ�F
					1.25�s�ڕt�߂ɂ���@�@setPinStatus(...);	���R�s�[���ĉ��ɑ��₷�B
					2.()�̒���(new �G���R�[�_��,'key',"�G���R�[�_��")���������ށB
						(key�͂���pin���L�[�{�[�h�ő��삷��Ƃ��̃L�[�ƂȂ邽�ߔ��p�p���Ƃ��邱��)
					3.�I��

	�s�������炷�ꍇ:
					1.25�s�ڕt�߂ɂ���@�@setPinStatus(...);�̂������炵���G���R�[�_�Ɠ������O�̂��̂�()���ɂ�����̂������B
					2.�I��

	�g����s����pin.hpp��
					class �G���R�[�_���@�̂��ƂɁ@Encoder�����Ă�����̂����ł���B
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
