#include "pwm.hpp"

int Pwm::setupPwmOut(
		float frequency,float duty){return _setupPwmOut(frequency,duty);
}

void Pwm::pwmWrite(float duty){
	_pwmWrite(duty);
}

float Pwm::pwmWrite(){
	return _pwmWrite();
}
