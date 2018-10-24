#ifndef MAIN_H_
#define MAIN_H_

#include "system.h"
#include "app.h"
#include "util.h"

#include "CatchRoboMain.h"
#include "CatchRoboMap.h"


System sys;

USART serial;

CAN can;
TIM pwm[8];
CanNodePulse servo[8];


uint64_t intervalTime;

void setup(){
	sys.setup();
	serial.setup(USART1_Setup,115200);
	sys.usartSetup(serial);

	//50Hz
	pwm[0].pwmSetup(TIM2_PWM1_Setup,20000,72);
	pwm[1].pwmSetup(TIM2_PWM2_Setup,20000,72);
	pwm[2].pwmSetup(TIM2_PWM3_Setup,20000,72);
	pwm[3].pwmSetup(TIM2_PWM4_Setup,20000,72);


	can.setup(CAN1_Setup);
	servo[0].setup(pwm[0],can,0);
	servo[1].setup(pwm[1],can,1);

}

extern "C" void USB_LP_CAN1_RX0_IRQHandler(void){
	can.receive();

	servo[0].interrupt();
	servo[1].interrupt();
	return;
}


#endif
