#ifndef _CAN_INTERFACE_H_
#define _CAN_INTERFACE_H_

#include "system.h"
#include "can.h"

class CAN_RX{
public:
	void setup(CAN &can,uint16_t _address,uint64_t _timeout = 100){
		this->can = &can;
		address = _address;
		timeout = _timeout;
		receiveFlag = 0;
		funcAttached = 0;
		can.filterAdd(address);
	}
	int isReceive(){
		if(receiveFlag){
			receiveFlag = 0;
			return 1;
		}else{
			return 0;
		}
	}
	
	int isTimeout(){
		if(millis() - lastReceiveTime > timeout){
			return -1;
		}else{
			return 0;
		}
	}
	
	void attachFunc(void (*po)()){
		intFunc = po;
		funcAttached = 1;
	}

	void interrupt(){
		if(can->rxMessage.StdId == address){
			for(int i=0;i<8;i++){
				data[i] = can->rxMessage.Data[i];
			}
			receiveFlag = 1;
			lastReceiveTime = millis();
			if(funcAttached)intFunc();
		}
	}

	uint8_t data[8];
	uint16_t address,receiveFlag,funcAttached;
	uint64_t lastReceiveTime,timeout;
	void (*intFunc)();
	CAN *can;
};

class CAN_TX{
public:
	void setup(CAN &can,uint16_t _address,uint64_t _interval = 100){
		this->can = &can;
		address = _address;
		interval = _interval;
		
		intervalTime = millis();
	}
	
	void send(uint8_t _data[]){
		can->send(address,8,_data);
		for(int i=0;i<8;i++){
			data[i] = _data[i];
		}
	}

	void send(){
		can->send(address,8,data);
	}

	void cycle(){
		if(millis() - intervalTime >= interval){
			intervalTime += interval;
			can->send(address,8,data);
		}
	}
	
	uint8_t data[8];
	uint16_t address;
	uint64_t intervalTime,interval;
	CAN *can;
};

#endif
