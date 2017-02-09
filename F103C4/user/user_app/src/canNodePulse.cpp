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
	return 1;
}




void CanNodePulse::interrupt(){
	if(canPulse_can->rxMessage.StdId == canPulse_address){
		outDuty = ((canPulse_can->rxMessage.Data[1] & 0b11111111) << 8) | canPulse_can->rxMessage.Data[0];
		if(outDuty > 700)outDuty = 700;
		if(outDuty < -700)outDuty = -700;
		canPulse_tim->duty(AMP_PULSE_CENTER + outDuty);	//一応動く
	}
}


//ここらへんの実装はサーボ基板に合わせる　ファイル名も合わせたほうがいいかも
