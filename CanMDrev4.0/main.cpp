
#include "main.hpp"

int main(void){
	uint32_t encOld[2] ={0,0};
	int32_t motorPPS[2] = {0,0};
	int32_t motorRV[2] = {0,0};

	setup();



	/*
	for(int i=0;i<20000;i++){
		currentOffset[0] += cs[0].read(ADC_SampleTime_239Cycles5);
		currentOffset[1] += cs[1].read(ADC_SampleTime_239Cycles5);
	}
	currentOffset[0] /= 20000;
	currentOffset[1] /= 20000;
	serial.printf("currentOffset = %6d,%6d\n\r",currentOffset[0],currentOffset[1]);

	cs[0].start(ADC_SampleTime_239Cycles5);
	cs[1].start(ADC_SampleTime_239Cycles5);

	TIM4ITSetup();*/


	while(1){
		if(vserial.available()){
			serial.send(vserial.read());
			led[0].write(Bit_SET);
			led[1].write(Bit_RESET);
		}else{
			led[0].write(Bit_RESET);
			led[1].write(Bit_SET);
		}


		if(sw[1].read() == 0){
			motor[0].duty(0.1);
			motor[1].duty(0.1);
		}else{
			if(sw[0].read() == 0){
				motor[0].duty(-0.1);
				motor[1].duty(-0.1);
			}else{
				motor[0].duty(0);
				motor[1].duty(0);
			}
		}

		if(printTime < millis()){
			printTime += PRINT_TIME;
			//serial.printf("%d,%d\n\r",motor[0].outDuty,motor[1].outDuty);
		}

	}

	//IWDGSetup(PRINT_TIME * 20);

    while(1){
    	nodeMotor[0].cycle();
    	nodeMotor[1].cycle();

    	if(cycleTime <= millis()){
    		cycleTime += CYCLE_TIME;
    		motorPPS[0] = ( ( encOld[0] - enc[0].read() ) );
    		motorPPS[1] = ( ( encOld[1] - enc[1].read() ) );

    		motorRV[0] = motorPPS[0]/MOTOR_REVOLUTION_PER_VOLTAGE;
    		motorRV[1] = motorPPS[1]/MOTOR_REVOLUTION_PER_VOLTAGE;


    		encOld[0] = enc[0].read();
    		encOld[1] = enc[1].read();
    	}

    	if(printTime <= millis()){

    		if(sw[0].read() == 0){
    			canMD[0].brake(1);
    			canMD[1].brake(1);
    		}else{
    			if(sw[1].read() ==1){
    				canMD[0].brake(0);
    				canMD[1].brake(0);
    			}else{
    				canMD[0].duty(-0.3);
    				canMD[1].duty(-0.3);
    			}

    		}

       		printTime = millis() + PRINT_TIME;
    		IWDGReset();

    		currentInt[0] /= currentCnt[0] / PRINT_TIME;
    		currentInt[1] /= currentCnt[1] / PRINT_TIME;
    		//serial.printf("%d,%d\n\r",canMD[0].outDuty16,nodeMotor[0].outDuty);
    		//serial.printf("%4f,%4f\n\r",nodeMotor[0].outDutyF,nodeMotor[1].outDutyF);
    		//serial.printf("%4d,%6d,%6d,%8d\n\r",(uint32_t)currentCnt[0],(int32_t)currentInt[0],(int32_t)currentInt[1],enc[0].read());

    		serial.printf("%d,%d,%d,%d\n\r",motorPPS[0],motorPPS[1],motorRV[0],motorRV[1]);

    		currentCnt[0] = 0;
    		currentCnt[1] = 0;
    		currentInt[0] = 0;
    		currentInt[1] = 0;
    	}

    	while(rxFlag > 0){
    		rxFlag--;
    		//serial.printf("ADD = %x,%d\n\r",can1.rxMessage.StdId,can1.rxMessage.Data[1]);
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

