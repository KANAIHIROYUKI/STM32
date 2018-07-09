#include "SBDBT.h"

void SBDBT::setup(USART_TypeDef *usart,GPIO_TypeDef* gpio_tx,uint16_t pin_tx,GPIO_TypeDef* gpio_rx,uint16_t pin_rx,uint32_t baud){
	serial.setup(usart,gpio_tx,pin_tx,gpio_rx,pin_rx,baud);

	error = 0;
	update = 0;
	lastReceiveTime = 0;

	for(int i=0;i<DataBufferSize;i++){
		data[i] = 0;
	}


	System::cycleFunctionNumber++;
}

void SBDBT::cycle(){
	while(serial.available()){
		uint8_t rxData = serial.read();

		if(rxData >> 7){		//スタートバイト
			rxNum = 0;
			parity = 0;
		}else if(rxNum < 2){
			for(int i=0;i<7;i++){
				if((rxData & (1<<i)) != 0){
					data[i + (rxNum*7)] = 1;
				}else{
					data[i + (rxNum*7)] = 0;
				}
			}
			parity += rxData;
			rxNum++;
		}else if(rxNum < 6){
			data[12 + rxNum] = (int)(rxData & 0x7F) - 0x40;
			rxNum++;
			parity += rxData;
		}else if(rxNum == 6){
			parity &= 0x7F;
			if(parity == rxData){
				lastReceiveTime = millis();
				update = 1;
			}else{
				error = 1;
			}
		}


	}


	System::cycleFunctionCnt--;
}


int16_t SBDBT::read(uint16_t address){
	if(address > DataBufferSize)return 0;
	return data[address];
}
