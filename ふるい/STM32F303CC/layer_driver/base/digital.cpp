#include "digital.hpp"

int Digital::setupDigitalIn(){
	int i=_setupDigitalIn();
	if(i)return i;
	_digitalMode=DIGITAL_MODE_IN;
	return 0;
}

int Digital::setupDigitalOut(){
	int i=_setupDigitalOut();
	if(i)return i;
	_digitalMode=DIGITAL_MODE_OUT;
	return 0;
}

int Digital::setupDigitalInPullUp(){
	int i=_setupDigitalInPullUp();
	if(i)return i;
	_digitalMode=DIGITAL_MODE_PULLUP;
	return 0;
}

int Digital::setupDigitalInPullDown(){
	int i=_setupDigitalInPullDown();
	if(i)return i;
	_digitalMode=DIGITAL_MODE_PULLDOWN;
	return 0;
}

void Digital::digitalWrite(int value){
	if(_digitalMode==DIGITAL_MODE_NULL) return;
	if(_digitalMode==DIGITAL_MODE_OUT)_digitalWrite(value);
	else{
		if(value==1)setupDigitalInPullUp();
		else if(value==0)setupDigitalInPullDown();
		else setupDigitalIn();
	}
}

int Digital::digitalRead(){
	if(_digitalMode==DIGITAL_MODE_NULL) return 0;
	if(_digitalMode==DIGITAL_MODE_IN||_digitalMode==DIGITAL_MODE_PULLUP||_digitalMode==DIGITAL_MODE_PULLDOWN)return _digitalRead();
	else return _digitalWrite();
}

void Digital::digitalHigh(){digitalWrite(1);}
void Digital::digitalLow(){digitalWrite(0);}
void Digital::digitalToggle(){digitalWrite(!digitalRead());}
