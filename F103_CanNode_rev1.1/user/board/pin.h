#ifndef _PIN_H_
#define _PIN_H_

#include "gpio.h"
#include "adc.h"
#include "can.h"

#include "app.h"

class Led:public GPIO{
public:
	Led(){
		setup(PB2,OUTPUT);
	}
};

/*******‚·‚×‚Ä“dŽ¥•Ù**********/

class Port0:public PORT{
public:
	Port0(){
		io[0].setup(PA2,OUTPUT);
		io[1].setup(PA3,OUTPUT);
		io[2].setup(PB0,OUTPUT);
		io[3].setup(PB1,OUTPUT);

		io[4].setup(PA4,OUTPUT);
		io[5].setup(PA5,OUTPUT);
		io[6].setup(PB12,OUTPUT);
		io[7].setup(PB13,OUTPUT);

		port_num = 7;
	}
};



class Can1:public CAN{
public:
	Can1(){
		GPIO canRX;
		GPIO canTX;
		canTX.setup(PA12,OUTPUT_AF);
		canRX.setup(PA11,INPUT);

		setup(CAN1);
	}
};

class Serial0:public USART{
public:
	Serial0(){
		GPIO usartTX;
		GPIO usartRX;

		usartTX.setup(PB10,OUTPUT_AF);
		usartRX.setup(PB11,INPUT);

		setup(USART3,921600);
	}
};
#endif
