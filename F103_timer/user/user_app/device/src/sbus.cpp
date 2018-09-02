#include "sbus.h"

void SBUS::setup(USART_TypeDef *usart,GPIO_TypeDef* gpio_tx,uint16_t pin_tx,GPIO_TypeDef* gpio_rx,uint16_t pin_rx){
	serial.setup(usart,gpio_tx,pin_tx,gpio_rx,pin_rx,100000);

	errorValue = 0;

	receiveIntervalTime = 0;
	lastReceiveTime = 0;
	update = 0;
	connect = 0;

	System::cycleFunctionNumber++;
}

void SBUS::cycle(){
	System::cycleFunctionCnt--;

	while(serial.available()){
		uint8_t data = serial.read();
		if(data == SBUS_START_BYTE)receiveBytes = 0;		//受信データカウンタはスタートバイトのみでクリア

		if(receiveBytes < SBUS_BUFFER_END)buffer[receiveBytes] = data;

		receiveBytes++;

	}

	//受信処理完了

	if(receiveBytes == SBUS_BUFFER_END){		//END_BYTEを受信
		channel[0]  = ((buffer[1]    |buffer[2]<<8)                 & 0x07FF);
		channel[1]  = ((buffer[2]>>3 |buffer[3]<<5)                 & 0x07FF);
		channel[2]  = ((buffer[3]>>6 |buffer[4]<<2 |buffer[5]<<10)  & 0x07FF);
		channel[3]  = ((buffer[5]>>1 |buffer[6]<<7)                 & 0x07FF);
		channel[4]  = ((buffer[6]>>4 |buffer[7]<<4)                 & 0x07FF);
		channel[5]  = ((buffer[7]>>7 |buffer[8]<<1 |buffer[9]<<9)   & 0x07FF);
		channel[6]  = ((buffer[9]>>2 |buffer[10]<<6)                & 0x07FF);
		channel[7]  = ((buffer[10]>>5|buffer[11]<<3)                & 0x07FF);
		channel[8]  = ((buffer[12]   |buffer[13]<<8)                & 0x07FF);
		channel[9]  = ((buffer[13]>>3|buffer[14]<<5)                & 0x07FF);
		channel[10] = ((buffer[14]>>6|buffer[15]<<2|buffer[16]<<10) & 0x07FF);
		channel[11] = ((buffer[16]>>1|buffer[17]<<7)                & 0x07FF);
		channel[12] = ((buffer[17]>>4|buffer[18]<<4)                & 0x07FF);
		channel[13] = ((buffer[18]>>7|buffer[19]<<1|buffer[20]<<9)  & 0x07FF);
		channel[14] = ((buffer[20]>>2|buffer[21]<<6)                & 0x07FF);
		channel[15] = ((buffer[21]>>5|buffer[22]<<3)                & 0x07FF);
		receiveBytes = 50;	//これしないとEND_BYTE受信してなくても一度受信すればそのままになってしまう

		((buffer[23])      & 0x0001) ? channel[16] = 2047: channel[16] = 0;
		((buffer[23] >> 1) & 0x0001) ? channel[17] = 2047: channel[17] = 0;

		if ((buffer[23] >> 3) & 0x0001) {
			connect = 0;
		} else {
			connect = 1;
		}

		if ((buffer[23] >> 2) & 0x0001) {
			frameOk = 0;
		} else {
			frameOk = 1;
		}


		receiveIntervalTime = millis() - lastReceiveTime;
		lastReceiveTime = millis();
		update = 1;
	}

	if(receiveBytes == SBUS_BUFFER_END + 1){
		errorValue |= SBUS_ERROR_FRAME;
	}
}

float SBUS::readF(uint16_t getChannel){
	if(getChannel >= 16)return 0;

	return (channel[getChannel] - 1024)/10.24;	//しらん
}

uint16_t SBUS::read(uint16_t getChannel){
	if(getChannel >= 16)return 0;

	return channel[getChannel];
}
