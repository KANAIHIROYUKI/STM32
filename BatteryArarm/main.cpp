#include "main.h"


int main(void)
{
	setup();



	while(1){
		//delay(10);

		voltage[0] = batt[0].read()*CELL0_VOLTAGE_GAIN;
		voltage[1] = batt[1].read()*CELL1_VOLTAGE_GAIN - voltage[0];
		voltage[2] = batt[2].read()*CELL2_VOLTAGE_GAIN - voltage[1];
		voltage[3] = batt[3].read()*CELL3_VOLTAGE_GAIN - voltage[2];
		voltage[4] = batt[4].read()*CELL4_VOLTAGE_GAIN - voltage[3];
		voltage[5] = batt[5].read()*CELL5_VOLTAGE_GAIN - voltage[4];

		vcnt++;
		for(int i=0;i<6;i++){
			vave[i] += voltage[i];
			if(voltage[i] < vmin[i])vmin[i] = voltage[i];
			if(voltage[i] > vmax[i])vmax[i] = voltage[i];
		}

		if(intervalTime < millis()){
			signal.toggle();
			power.toggle();

			intervalTime += IntervalTime;

			for(int i=0;i<6;i++){
				vave[i] = vave[i]/vcnt;

				if(vmax[0] < 3400){
					if(buzzerStat == 0)buzzerStat = 2;
				}else{
					buzzerStat = 0;
				}
			}

			serial.printf("%6d,%d\n\r",(uint32_t)vmax[0],buzzerStat);

			vcnt = 0;
			for(int i=0;i<6;i++){
				vmin[i] = 363000;
				vmax[i] = 0;
				vave[i] = 0;
			}

			switch(buzzerStat){
			case 0:
				beep(2500,0);
				buzzerStatCnt = 0;
				break;
			case 1:
				beep(2500,0.5);
				buzzerStatCnt = 0;
				break;
			case 2:
				beep(2500,0.1);
				buzzerStatCnt++;
				if(buzzerStatCnt > 10){
					buzzerStatCnt = 0;
					buzzerStat = 3;
				}
				break;
			case 3:
				beep(2500,0);
				buzzerStatCnt++;
				if(buzzerStatCnt > 10){
					buzzerStatCnt = 0;
					buzzerStat = 2;
				}
				break;
			case 4:
				break;
			case 5:
				break;
			default:
				break;
			}


		}
	}
}
