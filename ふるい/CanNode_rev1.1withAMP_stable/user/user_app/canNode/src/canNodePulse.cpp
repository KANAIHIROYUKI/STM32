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
	canPulse_mode = CAN_PULSE_MODE_TIM;

	System::cycleFunctionNumber++;
	//return 1;
}

int16_t CanNodePulse::setup(GPIO &gpio,CAN &can,uint16_t number){
	this->canPulse_can = &can;
	this->canPulse_gpio = &gpio;
	canPulse_address = CAN_ADD_DUTY + number;
	canPulse_can->filterAdd(canPulse_address);

	canPulse_mode = CAN_PULSE_MODE_IO;
	canPulse_gpio->write(Bit_RESET);
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
		outDuty = ((canPulse_can->rxMessage.Data[1] & 0b01111111) << 8) | canPulse_can->rxMessage.Data[0];
		if(canPulse_can->rxMessage.Data[1] & 0b10000000){
			outDuty = -outDuty;
		}
		outDuty = (float)((AMP_PULSE_MAX - AMP_PULSE_CENTER)*outDuty)/32767;
		if(outDuty > AMP_PULSE_LIMIT)outDuty = AMP_PULSE_LIMIT;
		if(outDuty < -AMP_PULSE_LIMIT)outDuty = -AMP_PULSE_LIMIT;


		if(canPulse_mode == CAN_PULSE_MODE_TIM)canPulse_tim->duty(AMP_PULSE_CENTER + outDuty);	//ˆê‰ž“®‚­
		if(canPulse_mode == CAN_PULSE_MODE_IO)canPulse_pulseDuty = AMP_PULSE_CENTER + outDuty;
		canPulse_receiveTime = millis();
	}
}

void CanNodePulse::cycle(){
	System::cycleFunctionCnt--;
	if(canPulse_mode == CAN_PULSE_MODE_TIM){
		if(canPulse_receiveTime + 100 < millis()){
			canPulse_tim->duty(AMP_PULSE_CENTER);
			outDuty = 0;
		}
		return;
	}
	if(canPulse_cycleTime <= micros()){
		canPulse_gpio->write(Bit_RESET);
		canPulse_pulseTime = canPulse_cycleTime + canPulse_pulseDuty;
		canPulse_cycleTime += AMP_PULSE_CYCLE;
	}
	if(canPulse_pulseTime <= micros()){
		canPulse_gpio->write(Bit_SET);
	}

	if(canPulse_receiveTime + 100 < millis()){
		canPulse_pulseDuty = 0;
	}
}
