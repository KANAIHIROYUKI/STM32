#include "SI8900.h"

void SI8900::setup(USART &usart,uint16_t modeSet){
	this->usart = &usart;
	mode = modeSet;
	readStat[0] = 0;
	readStat[1] = 0;
	readStat[2] = 0;

}

void SI8900::cycle(){
	if(usart->available()){
		uint8_t data = usart->read();
		if((data >> 6) == 0x03){		//CONFIG_0 CommandByte
			mxAddress = (data >> 4) & 0x03;

		}else if((data >> 6) == 0x02){	//ADC_H Byte
			mxAddress = (data >> 4) & 0x03;
			buffer = data & 0x0F;

		}else{							//ADC_L Byte
			buffer |= (data & 0x7E) << 4;
			value[mxAddress] = buffer;
			readStat[mxAddress] = 1;
			if(mode == SI8900_MODE_LOOP){
				request();
			}
		}
	}
}

void SI8900::request(uint16_t channel,uint16_t convert_mode){
	mode = convert_mode;
	uint8_t data;

	if(channel > 3)return;	//そんなチャンネルねーよばーか

	if(mode == SI8900_MODE_DEMAND){
		data = 0b11001011;					//VDD reference,demand mode,gain 1.0
		data |= (channel&3) << 4;
	}else{
		data = 0b11001001;					//VDD reference,demand mode,gain 1.0
	}
	USART_SendData(USART2,data);
	//usart->send(data);
}

uint16_t SI8900::read(uint16_t channel){
	readStat[channel] = 0;
	return value[channel];
}

uint16_t SI8900::stat(uint16_t channel){
	return readStat[channel];
}
