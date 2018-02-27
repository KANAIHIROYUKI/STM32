#include "system.h"
#include "SerialArduino.h"

class RS485_BUS{
public:
	void setup(USART_TypeDef *usart,uint32_t baud,GPIO_TypeDef* gpio_tx,uint16_t pin_tx,GPIO_TypeDef* gpio_rx,uint16_t pin_rx,GPIO &_dir){
		sa.setup(usart,baud,gpio_tx,pin_tx,gpio_rx,pin_rx);
		this->dir = &_dir;
		dir->write(0);

		sendStat = 0;
	}

	void cycle();

	int sendStat;
	USART *rs485;
	GPIO *dir;
	SerialArduino sa;
};
