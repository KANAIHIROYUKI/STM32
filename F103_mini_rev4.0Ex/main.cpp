#include "main.h"

int main(void)
{
	setup();

	uint64_t intervalTimer = 0;
	int64_t ave[3],min[3],max[3],readCnt,voltageValue;
	uint16_t adcValue[2];

	while(1){
		readCnt++;

		voltageValue = vbatt.read();
		ave[0] += voltageValue;
		if(voltageValue < min[0])min[0] = voltageValue;
		if(voltageValue > max[0])max[0] = voltageValue;

		adcValue[0] = v1a.read();
		adcValue[1] = v1b.read();
		voltageValue = adcValue[0] - adcValue[1];
		if(voltageValue < min[1])min[1] = voltageValue;
		if(voltageValue > max[1])max[1] = voltageValue;

		adcValue[0] = v2a.read();
		adcValue[1] = v2b.read();
		voltageValue = adcValue[0] - adcValue[1];
		if(voltageValue < min[2])min[2] = voltageValue;
		if(voltageValue > max[2])max[2] = voltageValue;

		if(intervalTimer < millis()){
			intervalTimer += IntervalTime;

			ave[0] /= readCnt;
			ave[1] /= readCnt;
			ave[2] /= readCnt;

			serial.printf("cnt = %d,Data = %d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n\r",readCnt,ave[0],ave[1],ave[2],min[0],min[1],min[2],max[0],max[1],max[2]);
			vserial.printf("%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n\r",ave[0],ave[1],ave[2],min[0],min[1],min[2],max[0],max[1],max[2]);

			ave[0] = 0;
			ave[1] = 0;
			ave[2] = 0;
			min[0] = 0xFFF;
			min[1] = 0xFFF;
			min[2] = 0;
			max[0] = 0;
			max[1] = 0;
			max[2] = 0;

			readCnt = 0;
		}




		//serial.printf("vbatt = %4d,v1a = %4d,bab = %4d,v2a = %4d,v2b = %4d\n\r",vbatt.read(),v1a.read(),v1b.read(),v2a.read(),v2b.read());


	}
}
