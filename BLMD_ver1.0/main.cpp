
#include "main.hpp"

int main(void)
{
	setup();

	sys.wdgSetup(50);	//5ms

	cs.itSetup();
	cs.start();

    while(1){
    	sys.cycle();
    	input[0].cycle();
    	input[1].cycle();
    	input[2].cycle();

    	if(printTime <= millis()){
    		led[0].toggle();
    		printTime = millis() + PRINT_TIME;

       		bldc.degree(millis());

       		serial.printf("%d,%d\n\r",adcCnt,adcValue);
       		adcCnt = 0;
       		adcValue = 0;
       		//cs.start(ADC_SampleTime_239Cycles5);
       		//serial.printf("%d,%d,%d\n\r",(uint32_t)timer.read(),(uint32_t)micros(),(uint32_t)systicMicros());
       		//serial.printf("%d,%d,%d,%d\n\r",sell[0].read(),sell[1].read(),sell[2].read(),sell[3].read());
       		//serial.printf("%d,%d,%d,%d,%d,%d\n\r",input[0].read(),(uint32_t)input[0].changeTime,input[1].read(),(uint32_t)input[1].changeTime,input[2].read(),(uint32_t)input[2].changeTime);

        	if(extiPinNumber != 0){
       			serial.printf("pin = %d,%d\n\r",extiPinNumber,(uint32_t)interruptTime);
       			extiPinNumber = 0;
       		}
    	}
    }
}

extern "C" void EXTI15_10_IRQHandler(void){
	interruptTime = timer.read();
	if(zeroCross[0].interruptFrag()){
		extiPinNumber = 12;
	}
	if(zeroCross[1].interruptFrag()){
		extiPinNumber = 13;
	}
	if(zeroCross[2].interruptFrag()){
		extiPinNumber = 14;
	}
}

extern "C" void ADC1_2_IRQHandler(void){
	adcCnt++;
	cs.interrupt();

	adcValue = cs.peek();
	cs.start(ADC_SampleTime_239Cycles5);
}
