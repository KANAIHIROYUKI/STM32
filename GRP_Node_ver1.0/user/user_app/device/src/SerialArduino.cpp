#include "SerialArduino.h"

void SerialArduino::setup(USART_TypeDef *usart,GPIO_TypeDef* gpio_tx,uint16_t pin_tx,GPIO_TypeDef* gpio_rx,uint16_t pin_rx,uint32_t baud){
	serial.setup(usart,gpio_tx,pin_tx,gpio_rx,pin_rx,baud);


	receiveStat = 0;
	sendRequest = 0;
	sendEnded = 0;
	update = 0;
	lastReceiveTime = 0;

	for(int i=0;i<SA_DATA_SIZE*3;i++){
		buffer[i] = 0;
		sendBuffer[i] = 0;
	}

	System::cycleFunctionNumber++;
}

void SerialArduino::cycle(){
	while(serial.available()){
		uint8_t data = serial.read();

		if(data >> 6 == SA_FRAME_START){				//スター
			frameSize = data & 0x3F;
			bufferAddress = 0;

		}else if(data >> 6 == SA_FRAME_DATA){			//データ
			buffer[bufferAddress] = data & 0x3F;
			bufferAddress++;

		}else if(data >> 6 == SA_FRAME_END){
			receiveStat = 1;


			if(bufferAddress != frameSize){
				//error
			}else{

			}
		}
	}

	if(receiveStat){
		for(int i=0;i<SA_DATA_SIZE;i++){
			data[i] = (buffer[i*3 + 2] << 12) | (buffer[i*3 + 1] << 6)  | buffer[i*3];
		}
		receiveStat = 0;
		lastReceiveTime = millis();
		update = 1;
	}

	if(sendRequest){
		serial.send(SA_DATA_SIZE*3 | SA_FRAME_START << 6);

		for(int i=0;i<SA_DATA_SIZE;i++){
			serial.send(sendBuffer[i*3    ] | (SA_FRAME_DATA << 6));
			serial.send(sendBuffer[i*3 + 1] | (SA_FRAME_DATA << 6));
			serial.send(sendBuffer[i*3 + 2] | (SA_FRAME_DATA << 6));
		}

		serial.send(SA_FRAME_END << 6);

		sendRequest = 0;
		sendEnded = 1;
	}

	System::cycleFunctionCnt--;
}

void SerialArduino::write(uint16_t address,int16_t data){
	if(address >= SA_DATA_SIZE)return;

	sendRequest = 1;
	sendBuffer[address*3  ]   = data      & 0x3F;
	sendBuffer[address*3 + 1] = data >>6  & 0x3F;
	sendBuffer[address*3 + 2] = data >>12 & 0x3F;
}

int16_t SerialArduino::read(uint16_t address){
	if(address > SA_DATA_SIZE)return 0;
	return data[address];
}
