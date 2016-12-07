#include "pwm_test.hpp"

/*pwm_testの編集
	前提：pin.hppとpin.cppを基盤に合わせて編集を終わらせる。

	ピンを増やす場合：
					1.22行目付近にある　　setPinStatus(...);	をコピーして下に増やす。
					2.()の中に(new ピン名,'key',"ピン名")を書き込む。
						(keyはそのpinをキーボードで操作するときのキーとなるため半角英数とすること)
					3.終了
	ピンを減らす場合:
					1.22行目付近にある　　setPinStatus(...);のうち減らしたpinと同じ名前のものが()内にあるものを消す。
					2.終了

	使えるピンはpin.hppで
					class ピン名　のあとに　Pwmがついているものだけである。
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
