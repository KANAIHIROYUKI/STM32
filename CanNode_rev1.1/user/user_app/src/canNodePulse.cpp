#include "canNodePulse.h"


int16_t CanPulse::setup(TIM &tim,CAN &can,uint16_t number){
	this->canPulse_can = &can;
	this->canPulse_tim = &tim;
	canPulse_address = CAN_ADD_POSITION + number;
	canPulse_can->filterAdd(canPulse_address);

	if(canPulse_tim->pwm_prescaler == 72 && canPulse_tim->pwm_period == 10000){
		canPulse_tim->duty(AMP_PULSE_CENTER);
		return 0;
	}
	return 1;
}




void CanPulse::interrupt(){
	if(canPulse_can->rxMessage.StdId == canPulse_address){
		canPulse_tim->duty(canPulse_can->rxMessage.Data[0]);	//動かないよ
	}
}


//ここらへんの実装はサーボ基板に合わせる　ファイル名も合わせたほうがいいかも
