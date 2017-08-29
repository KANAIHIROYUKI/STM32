#include "canNodePulse.h"


int16_t CanNodePulse::setup(TIM &tim,CAN &can,uint16_t number){
	this->canPulse_can = &can;
	this->canPulse_tim = &tim;
	//if(canPulse_tim->pwm_prescaler == 72 && canPulse_tim->pwm_period == 20000){
		canPulse_address = CAN_ADD_DUTY + number;
		canPulse_can->filterAdd(canPulse_address);
		canPulse_tim->duty(AMP_PULSE_CENTER);
		return 0;
	//}
	//return 1;
}

int16_t CanNodePulse::setup(GPIO &gpio,CAN &can,uint16_t number){
	this->canPulse_can = &can;
	this->canPulse_gpio = &gpio;
	canPulse_address = CAN_ADD_DUTY + number;
	canPulse_can->filterAdd(canPulse_address);

	canPulse_mode = CAN_PULSE_MODE_IO;
	canPulse_gpio->write(0);
	canPulse_cycleTime = micros();

	canPulse_pulseDuty = AMP_PULSE_CENTER;

	System::cycleFunctionNumber++;

	cycle();
	return 0;
}

void CanNodePulse::invert(){
	//canPulse_gpio->toggle();
}


void CanNodePulse::interrupt(){
	if(canPulse_can->rxMessage.StdId == canPulse_address){
		outDuty = ((canPulse_can->rxMessage.Data[1] & 0b11111111) << 8) | canPulse_can->rxMessage.Data[0];
		if(outDuty > 700)outDuty = 700;
		if(outDuty < -700)outDuty = -700;

		if(canPulse_mode == CAN_PULSE_MODE_TIM)canPulse_tim->duty(AMP_PULSE_CENTER + outDuty);	//ˆê‰ž“®‚­
		if(canPulse_mode == CAN_PULSE_MODE_IO)canPulse_pulseDuty = AMP_PULSE_CENTER + outDuty;
	}
}

void CanNodePulse::cycle(){
	System::cycleFunctionCnt--;

	if(canPulse_mode == CAN_PULSE_MODE_TIM){
		return;
	}
	if(canPulse_cycleTime <= micros()){
		//canPulse_gpio->toggle();
		canPulse_gpio->write(0);
		canPulse_pulseTime = canPulse_cycleTime + canPulse_pulseDuty;
		canPulse_cycleTime += AMP_PULSE_CYCLE;
	}
	if(canPulse_pulseTime <= micros()){
		canPulse_gpio->write(1);
		//canPulse_gpio->toggle();
	}
}
