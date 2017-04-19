#include "SI8900.h"

void SI8900::setup(USART &usart,uint16_t modeSet){
	this->usart = &usart;
	mode = modeSet;
	mxCount = 0;
	mxAddress = 0;
	readStat[0] = 0;
	readStat[1] = 0;
	readStat[2] = 0;

	value[0] = 0;
	value[1] = 0;
	value[2] = 0;
	receiveError = 0;

	if(timingCalibration())return;

	if(mode == SI8900_MODE_LOOP)request(0,SI8900_MODE_LOOP);
}

void SI8900::cycle(){
	if(usart->available()){
		uint8_t data = usart->read();
		if((data >> 6) == 0x03){		//CONFIG_0 CommandByte
			if((data >> 2) & 1 != 0)receiveError = 1;
			mxAddress = (data >> 4) & 0x03;
			receiveTime[mxAddress] = millis();

		}else if((data >> 6) == 0x02){	//ADC_H Byte
			mxAddress = (data >> 4) & 0x03;

			buffer = (data & 0x0F) << 6;
		}else{							//ADC_L Byte
			buffer |= (data & 0x7E) >> 1;

			value[mxAddress] = buffer;
			readStat[mxAddress] = 1;

			if(mode == SI8900_MODE_LOOP){
				mxCount++;
				if(mxCount > 2)mxCount = 0;
				request(mxCount,SI8900_MODE_LOOP);
			}
		}
	}
}

void SI8900::request(uint16_t channel,uint16_t convert_mode){
	mode = convert_mode;
	uint8_t data;

	if(channel > 2)return;	//そんなチャンネルねーよばーか

	if(mode == SI8900_MODE_BURST){
		data = 0b11001001;					//VDD reference,burst mode,gain 1.0
	}else{
		data = 0b11001011;					//VDD reference,demand mode,gain 1.0
		data |= (channel&3) << 4;
	}
	USART_SendData(usart->usart_usart,data);
	//usart->send(data);
}

uint16_t SI8900::read(uint16_t channel){
	readStat[channel] = 0;
	return value[channel];
}

uint16_t SI8900::stat(uint16_t channel){
	return readStat[channel];
}

uint16_t SI8900::timingCalibration(uint64_t timeOut){
	receiveError = 0;

	USART_SendData(usart->usart_usart,0xAA);
	for(int i=0;i<timeOut;i++){
		while(usart->available()){
			if(usart->read() == 0x55){
				return 0;
			}
		}
		delay(1);
		USART_SendData(usart->usart_usart,0xAA);
	}
}
