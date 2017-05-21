#include "main.h"


int main(void)
{
	setup();

	while(1){
		//delay(10);

		voltage[0] = (batt[0].read()*CELL0_VOLTAGE_GAIN);
		voltage[1] = (batt[1].read()*CELL1_VOLTAGE_GAIN);
		voltage[2] = (batt[2].read()*CELL2_VOLTAGE_GAIN);
		voltage[3] = (batt[3].read()*CELL3_VOLTAGE_GAIN);
		voltage[4] = (batt[4].read()*CELL4_VOLTAGE_GAIN);
		voltage[5] = (batt[5].read()*CELL5_VOLTAGE_GAIN);

		battv = voltage[5];
/*
		voltage[5] -= voltage[4];
		voltage[4] -= voltage[3];
		voltage[3] -= voltage[2];
		voltage[2] -= voltage[1];
		voltage[1] -= voltage[0];*/
/*
		vcnt++;
		for(int i=0;i<6;i++){
			vave[i] += voltage[i];
			if(voltage[i] < vmin[i])vmin[i] = voltage[i];
			if(voltage[i] > vmax[i])vmax[i] = voltage[i];
		}*/

		if(intervalTime < millis()){

			intervalTime += IntervalTime;

			/*serial.printf("\n\r");
			for(int i=0;i<6;i++){
				vave[i] = vave[i]/vcnt;
				serial.printf("%6d,",(uint32_t)voltage[i]);
			}*/

			//すべてのセルの最大電圧と最低電圧を出す
			/*
			cellMin = 36300;
			cellMax = 0;
			for(int i=0;i<6;i++){
				if(cellMin > voltage[i])cellMin = voltage[i];
				if(cellMax < voltage[i])cellMax = voltage[i];
			}
*/
			//セル電圧差の最大値を出す
			/*
			cellWorst = cellMax - cellMin;
			*/

			//battv = 23000;

			if(battv < 23000){
				beepEnable = 1;
			}else{
				beepEnable = 0;
			}

			beepInterval =  (battv - ((cellNum+1) * 3600))/10;
			if(beepInterval < 100)beepInterval = 10;
			serial.printf("%d,%d,%d\n\r",(uint32_t)battv,beepInterval,buzzerStatCnt);

			if(buzzerStatCnt == 0){
				if(beepEnable)beep(4000,0.5);
			}
			buzzerStatCnt++;
			if(buzzerStatCnt > beepInterval){	//beepIntervalに達するまで鳴らさない
				buzzerStatCnt = 0;
			}else if(buzzerStatCnt == 5){		//beepIntervalに関係なく鳴らす時間は一定
				beep(5000,0);
			}

			vcnt = 0;



		}

		if(millis() - ledInterval > 800){
			ledInterval = millis();
			power.write(1);
			delay(10);
			power.write(0);
		}
	}
}
