#include "main.h"


int main(void)
{
	setup();

	uint8_t switchState,switchStateOld,canData[8],stateCnt,sendState;



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
			sendState = switchState;	//定期送信用にチャタリング対策済みの値を代入
			transmitTime = millis() + transmitIntervalTime;
			//serial.printf("%x\n\r",switchState);

			canData[0] = sendState;
			CAN1Send(CAN_ADD_SWITCH_VALUE,1,canData);
		}else if(stateCnt != 0){
			stateCnt--;
		}

		if((transmitIntervalTime != 0) && (transmitTime < millis())){
			transmitTime = millis() + transmitIntervalTime;
			canData[0] = sendState;
			//serial.printf("send\n\r");
			CAN1Send(CAN_ADD_SWITCH_VALUE,1,canData);
		}

		if(intervalTime < millis()){
			//signal.toggle();
			intervalTime = millis() + IntervalTime;
			//serial.printf("%d\n\r",canSwitch.read());
			serial.printf("%d,%d,%d,%d,%d,%2x\n\r",ROTARY,switchPin[4].read(),switchPin[5].read(),switchPin[6].read(),switchPin[7].read(),switchState);
		}

		if(rxFlag != 0){
			signal.toggle();
			rxFlag--;
			//serial.printf("%d,",transmitIntervalTime);
			serial.printf("CAN RX d0=%x,d1=%x,d2=%x\n\r",can1.rxMessage.Data[0],can1.rxMessage.Data[1],can1.rxMessage.Data[2]);
		}
	}
}
