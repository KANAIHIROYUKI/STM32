#include "main.hpp"

int main(void)
{
	setup();

	serial.setup(USART3,921600,PB10,PB11);
	serial.printf("\n\rFILE = %s\n\rDATE = %s\n\rTIME = %s\n\rADD = %d\n\r",__FILE__,__DATE__,__TIME__,CAN_ADD);

	//RCC_ADCCLKConfig(RCC_PCLK2_Div8);
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

    		if(sw[0].read() == 0){
    			canData[1] = 0b01111111;
    		}else{
    			canData[1] = 0b11111111;
    		}

    		canData[0] = 0b11111111;

    		if(sw[1].read() == 0){

    		}else{
    			can1.send(CAN_ADD_MOTOR + CAN_ADD,2,canData);
    			can1.send(CAN_ADD_MOTOR + CAN_ADD + 1,2,canData);
    		}

    		//serial.printf("%d,%d\n\r",motor[0].outDuty,motor[1].outDuty);
    		serial.printf("%d,%d,%d\n\r",currentCnt[0],currentInt[0],currentInt[1]);

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

extern "C" void USB_LP_CAN1_RX0_IRQHandler(void){
	rxFlag++;
	can1.receive(&rxMessage);
	canMD[0].interrupt(rxMessage);
	canMD[1].interrupt(rxMessage);

	return;

}

void TIM4_PWM_Update_IRQ(){
	if(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC) == SET){
		currentInt[0] += cs[0].peek();
		currentCnt[0]++;
		cs[0].start(ADC_SampleTime_239Cycles5);
	}

	if(ADC_GetFlagStatus(ADC2,ADC_FLAG_EOC) == SET){
		currentInt[1] += cs[1].peek();
		currentCnt[1]++;
		cs[1].start(ADC_SampleTime_239Cycles5);
	}
}
