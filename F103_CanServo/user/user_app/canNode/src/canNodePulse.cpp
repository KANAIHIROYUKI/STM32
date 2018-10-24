#include "canNodePulse.h"


int16_t CanNodePulse::setup(TIM &_tim,CAN &_can,uint16_t number){
	this->can = &_can;
	this->tim = &_tim;
	//if(tim->pwm_prescaler == 72 && tim->pwm_period == 20000){
		address = CAN_ADD_DUTY + number;
		can->filterAdd(address);
		tim->duty(AMP_PULSE_CENTER);
		return 0;
	//}
	//return 1;
}

int16_t CanNodePulse::setup(GPIO &_gpio,CAN &_can,uint16_t number){
	this->can = &_can;
	this->gpio = &_gpio;
	address = CAN_ADD_DUTY + number;
	can->filterAdd(address);

	mode = CAN_PULSE_MODE_IO;
	gpio->write(0);
	cycleTime = micros();

	pulseDuty = AMP_PULSE_CENTER;

	System::cycleFunctionNumber++;

	cycle();
	return 0;
}

void CanNodePulse::invert(){
	//gpio->toggle();
}


void CanNodePulse::interrupt(){
	if(can->rxMessage.StdId == address){
		//outDuty = (int)(uchar2_to_float(can->rxMessage.Data) * 700);
		outDuty = ((can->rxMessage.Data[1] & 0b01111111) << 8) | can->rxMessage.Data[0];
		if((can->rxMessage.Data[1] >> 7) == 1){
			outDuty = -outDuty;
		}
		outDuty = outDuty/45;
		if(outDuty > 700)outDuty = 700;
		if(outDuty < -700)outDuty = -700;

		if(mode == CAN_PULSE_MODE_TIM)tim->duty(AMP_PULSE_CENTER + outDuty);	//ˆê‰ž“®‚­
		if(mode == CAN_PULSE_MODE_IO)pulseDuty = AMP_PULSE_CENTER + outDuty;
	}
}

void CanNodePulse::cycle(){
	System::cycleFunctionCnt--;

	if(mode == CAN_PULSE_MODE_TIM){
		return;
	}
	if(cycleTime <= micros()){
		//gpio->toggle();
		gpio->write(0);
		pulseTime = cycleTime + pulseDuty;
		cycleTime += AMP_PULSE_CYCLE;
	}
	if(pulseTime <= micros()){
		gpio->write(1);
		//gpio->toggle();
	}
}
