#include "SI8900.h"

void SI8900::setup(USART &usart,uint16_t modeSet){
	this->usart = &usart;
	mode = modeSet;

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

	gpioSetuped = 0;

	if(mode == SI8900_MODE_LOOP)request(0,SI8900_MODE_LOOP);

	System::cycleFunctionNumber++;
}

void SI8900::gpioAssigne(GPIO &powerOnSet,GPIO &reset){
	this->powerOn = &powerOnSet;
	this->adcReset = &reset;

	adcReset->write(0);
	gpioSetuped = 1;
	resetCnt = 0;
}

void SI8900::cycle(){
	System::cycleFunctionCnt--;

	for(int i=0;i<3;i++){
		if(readStat[i] == 2)readStat[i] = 0;
	}

	if(gpioSetuped && powerOn->read() == 1){	//電源が来ていない
		setupStat = 0;							//当然セットアップできないわな
		if(usart->available())usart->read();	//
		return;
	}


	if(setupStat == 0){										//ボーレート設定処理
		if(usart->available()){
			setupData = usart->read();
			if(setupData == 0x55){
				setupStat = 1;

				if(gpioSetuped)adcReset->write(0);

				receiveTime[0] = millis();
				receiveTime[1] = millis();
				receiveTime[2] = millis();

				value[0] = 0;
				value[1] = 0;
				value[2] = 0;

				readStat[0] = 0;
				readStat[1] = 0;
				readStat[2] = 0;

			}
		}

		if(millis() - setupLastSendTime > 0){
			if(setupSendCnt < 200){
				if(gpioSetuped && setupSendCnt == 10)adcReset->write(0);						//リセット解除
				setupLastSendTime = millis();
				USART_SendData(usart->usart_usart,0xAA);
				setupSendCnt++;

			}else{
				setupSendCnt = 0;
				resetCnt++;
				if(gpioSetuped)adcReset->write(1);						//100回やっても初期設定できなかったらリセットかける(1ms)
			}
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
	if(readStat[channel])readStat[channel] = 2;
	return value[channel];
}

uint16_t SI8900::stat(uint16_t channel){
	return readStat[channel];
}
