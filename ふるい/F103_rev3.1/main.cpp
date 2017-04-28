#include "main.hpp"

int main(void)
{
	setup();

	//RCC_ADCCLKConfig(RCC_PCLK2_Div8);

	for(int i=0;i<1000;i++){
		currentOffset[0] += cs[0].read();
		currentOffset[1] += cs[1].read();
	}
	currentOffset[0] /= 1000;
	currentOffset[1] /= 1000;
	serial.printf("currentOffset = %6d,%6d\n\r",currentOffset[0],currentOffset[1]);

	cs[0].start(ADC_SampleTime_239Cycles5);
	cs[1].start(ADC_SampleTime_239Cycles5);

	TIM4ITSetup();

	IWDGSetup(PRINT_TIME * 20);

    while(1){
    	canMD[0].cycle();
    	canMD[1].cycle();

    	if(printTime <= millis()){
       		printTime = millis() + PRINT_TIME;
    		IWDGReset();


    		currentInt[0] /= currentCnt[0];
    		currentInt[1] /= currentCnt[1];
    		//serial.printf("%d,%d\n\r",motor[0].outDuty,motor[1].outDuty);
    		serial.printf("%4d,%6d,%6d,%8d\n\r",(uint32_t)currentCnt[0],(int32_t)currentInt[0],(int32_t)currentInt[1],enc[0].read());

    		currentCnt[0] = 0;
    		currentCnt[1] = 0;
    		currentInt[0] = 0;
    		currentInt[1] = 0;
    	}

    	while(rxFlag > 0){
    		rxFlag--;
    	}
    }
}



void TIM4_PWM_Update_IRQ(){
	if(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC) == SET){
		currentInt[0] += cs[0].peek() - currentOffset[0];
		currentCnt[0]++;
		cs[0].start(ADC_SampleTime_239Cycles5);
	}

	if(ADC_GetFlagStatus(ADC2,ADC_FLAG_EOC) == SET){
		currentInt[1] += cs[1].peek() - currentOffset[1];
		currentCnt[1]++;
		cs[1].start(ADC_SampleTime_239Cycles5);
	}
}
