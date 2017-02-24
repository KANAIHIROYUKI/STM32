#include "main.h"


int main(void)
{
	setup();

	uint8_t switchState,switchStateOld,canData[2],stateCnt;

	while(1){
		switchStateOld = switchState;
		switchState = 0;
		for(int i=0;i<8;i++){
			switchState += switchPin[i].read() << i;
		}

		if(switchState != switchStateOld){
			stateCnt = 255;
		}else if(stateCnt == 1){
			stateCnt = 0;
			canData[0] = switchState;
			CAN1Send(CAN_ADD_SWITCH,2,canData);
		}else if(stateCnt != 0){
			stateCnt--;
		}

		if(intervalTime < millis()){
			intervalTime = millis() + IntervalTime;
			serial.printf("%d,%d,%d,%d,%d,%2x\n\r",ROTARY,switchPin[4].read(),switchPin[5].read(),switchPin[6].read(),switchPin[7].read(),switchState);
		}

		if(rxFlag != 0){
			rxFlag--;
			serial.printf("CAN RX %x,%x,%x\n\r",can1.rxMessage.StdId,can1.rxMessage.Data[0],can1.rxMessage.Data[1]);
		}

	}
}
