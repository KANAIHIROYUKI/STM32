#include "pwm_test.hpp"

/*pwm_test�̕ҏW
	�O��Fpin.hpp��pin.cpp����Ղɍ��킹�ĕҏW���I��点��B

	�s���𑝂₷�ꍇ�F
					1.22�s�ڕt�߂ɂ���@�@setPinStatus(...);	���R�s�[���ĉ��ɑ��₷�B
					2.()�̒���(new �s����,'key',"�s����")���������ށB
						(key�͂���pin���L�[�{�[�h�ő��삷��Ƃ��̃L�[�ƂȂ邽�ߔ��p�p���Ƃ��邱��)
					3.�I��
	�s�������炷�ꍇ:
					1.22�s�ڕt�߂ɂ���@�@setPinStatus(...);�̂������炵��pin�Ɠ������O�̂��̂�()���ɂ�����̂������B
					2.�I��

	�g����s����pin.hpp��
					class �s�����@�̂��ƂɁ@Pwm�����Ă�����̂����ł���B
					ex)class A0:public Pwm{...};
 */

PwmTest::PwmTest(Console &console):ConsoleCommand(console){
	pin=0;
	setPinStatus(new Pwm0,'0',(char*)"Pwm0");
	setPinStatus(new Pwm1,'1',(char*)"Pwm1");
	setPinStatus(new Pwm2,'2',(char*)"Pwm2");
	setPinStatus(new Pwm3,'3',(char*)"Pwm3");
	setPinStatus(new Pwm4,'4',(char*)"Pwm4");
	pinData=0;
}

void PwmTest::setPinStatus(Pwm *pwm,char key,char *pinName){
	pwmPin[pin].pwm=pwm;
	pwmPin[pin].key=key;
	strcpy(pwmPin[pin].pinName,pinName);
	pin++;
}

int PwmTest::commandRequest(int argc,char **argv){
	int i=0;
	int pinnum=0;
	int frequency=0;
	float value=0;
	if(pinData==0){
		pinData++;
		for(i=0;i<pin;i++){
			pwmPin[i].pwm->setupPwmOut(10000,0);
			consolePrintf((char*)"'%c'key:%s\r\n",pwmPin[i].key,pwmPin[i].pinName);
		}
		consolePrintf((char*)"set frequency 100->'pwm 0 freq 100'\r\n");
		consolePrintf((char*)"set    duty   0.5->'pwm 0 duty Duty'\r\n");
		consolePrintf((char*)"DutyMin=0.0	DutyMax=1.0\r\n");
		consolePrintf((char*)"Pwm Mode\r\n");
	}
	for(i=0;i<pin;i++){
		pwmPin[i].pwm->setupPwmOut(10000,0);
	}
	if(argc>=2){
		for(i=0;i<pin;i++){
			if(pwmPin[i].key==argv[1][0]){
				pinnum=i;
			}
		}
		if(argc>=4){
			if(!strcmp(argv[2],"duty")){
				value=atof(argv[3]);
				pwmPin[pinnum].pwm->pwmWrite(value);
				consolePrintf((char*)"set duty %f\r\n",value);
			}
			if(!strcmp(argv[2],"freq")||!strcmp(argv[2],"frequency")){
				frequency=atoi(argv[3]);
				pwmPin[pinnum].pwm->setupPwmOut(frequency,0);
				consolePrintf((char*)"set frequency %d\r\n",frequency);
			}
		}
	}
	return 0;
}
