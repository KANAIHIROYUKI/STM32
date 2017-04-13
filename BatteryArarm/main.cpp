#include "main.h"


int main(void)
{
	setup();
	uint64_t setTime = millis();
	buzzerStat = 0;

	for(int i=0;i<20;i++){
		beep(i*500,0.5);
		delay(500);
		serial.printf("%d\n\r",i);
	}

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

			power.toggle();

			intervalTime += IntervalTime;

			hc04.printf("\n\r");
			for(int i=0;i<6;i++){
				vave[i] = vave[i]/vcnt;
				hc04.printf("%6d,",voltage[i]);
			}

			//すべてのセルの最大電圧と最低電圧を出す
			cellMin = 36300;
			cellMax = 0;
			for(int i=0;i<6;i++){
				if(cellMin > voltage[i])cellMin = voltage[i];
				if(cellMax < voltage[i])cellMax = voltage[i];
			}

			//セル電圧差の最大値を出す
			cellWorst = cellMax - cellMin;



			serial.printf("max = %6d,stat = %d\n\r",(uint32_t)vmax[0],buzzerStat);


			vcnt = 0;
			for(int i=0;i<6;i++){
				vmin[i] = 363000;
				vmax[i] = 0;
				vave[i] = 0;
			}

			if(millis() > setTime){
				setTime+=5000;
				buzzerStat++;
			}

			switch(buzzerStat){
			case 0:
				beep(2000,0);
				buzzerStatCnt = 0;
				break;
			case 1:
				if(buzzerStatCnt == 0)beep(2000,0.5);
				buzzerStatCnt++;
				if(buzzerStatCnt > 50){
					buzzerStatCnt = 0;
				}else if(buzzerStatCnt > 1){
					beep(5000,0);
				}
				break;
			case 2:
				if(buzzerStatCnt == 0)beep(2000,0.5);
				buzzerStatCnt++;
				if(buzzerStatCnt > 13){
					buzzerStatCnt = 0;
				}else if(buzzerStatCnt > 10){
					beep(2000,0);
				}
				break;
			case 3:
				if(buzzerStatCnt == 0)beep(2000,0.5);
				buzzerStatCnt++;
				if(buzzerStatCnt > 20){
					buzzerStatCnt = 0;
				}else if(buzzerStatCnt > 2){
					beep(2000,0);
				}
				break;
			case 4:
				if(buzzerStatCnt == 0)beep(2000,0.5);
				buzzerStatCnt++;
				if(buzzerStatCnt > 100){
					buzzerStatCnt = 0;
				}else if(buzzerStatCnt > 2){
					beep(2000,0);
				}
				break;
			case 5:
				if(buzzerStatCnt == 0)beep(2000,0.5);
				buzzerStatCnt++;
				if(buzzerStatCnt > 100){
					buzzerStatCnt = 0;
				}else if(buzzerStatCnt > 2){
					beep(2000,0);
				}
				break;
			default:
				beep(2000,0);
				buzzerStatCnt = 0;
				break;
			}

		}
	}
}
