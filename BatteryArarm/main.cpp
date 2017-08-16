#include "main.h"


int main(void)
{
	setup();

	while(1){

		voltage[0] = (batt[0].read()*CELL0_VOLTAGE_GAIN);
		voltage[1] = (batt[1].read()*CELL1_VOLTAGE_GAIN);
		voltage[2] = (batt[2].read()*CELL2_VOLTAGE_GAIN);
		voltage[3] = (batt[3].read()*CELL3_VOLTAGE_GAIN);
		voltage[4] = (batt[4].read()*CELL4_VOLTAGE_GAIN);
		voltage[5] = (batt[5].read()*CELL5_VOLTAGE_GAIN);

		battv = voltage[cellNum - 1];

		if(intervalTimer < millis()){
			intervalTimer += IntervalTime;

			voltageDifference =  (battv - (cellNum * 3600))/cellNum;

			if(voltageDifference < 0){
				underVoltageCnt++;
			}else{
				underVoltageCnt = 0;
			}

			if(underVoltageCnt > 50){	//500msà»è„3.6VÇâ∫âÒÇ¡ÇΩÇÁ
				beepEnable = 1;
			}

			ledIntervalTime = (voltageDifference + 100)/2;
			if(ledIntervalTime < 50)ledIntervalTime = 50;

			serial.printf("%d,%d,%d\n\r",(uint32_t)battv,voltageDifference,ledIntervalTime);
		}

		if(millis() > ledIntervalTimer){
			ledIntervalTimer += ledIntervalTime;
			power.toggle();
		}

		if(millis() > beepIntervalTimer){
			beepIntervalTimer += BeepIntervalTime;
			if(beepEnable)beep(4000,0.5);
		}
	}
}
