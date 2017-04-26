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

	receiveTime[0] = millis();
	receiveTime[1] = millis();
	receiveTime[2] = millis();

	requestTime = 0;

	setupLastSendTime = millis();
	setupStat = 0;

	if(mode == SI8900_MODE_LOOP)request(0,SI8900_MODE_LOOP);
}

void SI8900::cycle(){
	if(setupStat == 0){										//ボーレート設定処理
		if(usart->available()){
			setupData = usart->read();
			if(setupData == 0x55){
				setupStat = 1;
				receiveTime[0] = millis();
				receiveTime[1] = millis();
				receiveTime[2] = millis();
			}
		}

		if(millis() - setupLastSendTime > 0){
			setupLastSendTime = millis();
			USART_SendData(usart->usart_usart,0xAA);
		}
		return;
	}


	if(usart->available()){									//送受信処理

		uint8_t data = usart->read();

		if((data >> 6) == 0x03){							//CONFIG_0 CommandByte
			mxAddress = (data >> 4) & 0x03;

			if((((data >> 2) & 1) != 0) && (mxAddress  != requestChannel)){
				setupStat = 0;
				return;
			}

			receiveTime[mxAddress] = millis();

		}else if((data >> 6) == 0x02){						//ADC_H Byte
			mxAddress = (data >> 4) & 0x03;
			if(mxAddress != requestChannel){
				setupStat = 0;
				return;
			}

			buffer = (data & 0x0F) << 6;
			//buffer = 0;
		}else{												//ADC_L Byte
			// if((data & 0x81) == 0)

			buffer |= (data & 0x7E) >> 1;

			receiveTime[requestChannel] = millis();
			requestTime = 0;

			value[requestChannel] = buffer;
			readStat[requestChannel] = 1;

			if(mode == SI8900_MODE_LOOP){
				requestChannel++;
				if(requestChannel > 2)requestChannel = 0;
				request(requestChannel,SI8900_MODE_LOOP);
			}
		}
	}

	if(mode == SI8900_MODE_LOOP){						//通信切れ処理(他のモードだと検出しにくいのでLOOPモードでしか動かない)
		for(int i=0;i<3;i++){
			if(millis() - receiveTime[i] > 100){
				setupStat = 0;
			}
		}
	}
}

void SI8900::request(uint16_t channel,uint16_t convert_mode){
	mode = convert_mode;
	requestChannel = channel;
	uint8_t data;

	if(channel > 2)return;	//そんなチャンネルねーよばーか

	if(mode == SI8900_MODE_BURST){
		data = 0b11001001;					//VDD reference,burst mode,gain 1.0
	}else{
		data = 0b11001011;					//VDD reference,demand mode,gain 1.0
		data |= (requestChannel&3) << 4;
	}
	requestTime = millis();
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

	for(uint sendCnt=0;sendCnt<timeOut;sendCnt++){
		for(int i=0;i<5;i++){
			if(usart->read() == 0x55){
				setupStat = 1;
				return 0;
			}
		}
		delay(1);

	}

	for(int i=0;i<10;i++){					//送り続けても意味なさそうだから変なデータ送ってズラす(ここはいっていない気がする
		delay(1);
		USART_SendData(usart->usart_usart,0xCC);
	}
	return 1;
}
