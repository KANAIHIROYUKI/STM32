
#include "main.hpp"

int main(void)
{
	setup();



	if(canNodeMode == CAN_NODE_MODE_AMP){
		serial.printf("can node amp & encoder\n\r");
	}

	if(canNodeMode == CAN_NODE_MODE_VALVE){
		serial.printf("can node encoder & valve\n\r");
	}

	sys.wdgSetup(50);	//5ms
    while(1){
    	sys.cycle();

    	if(canNodeMode == CAN_NODE_MODE_AMP){
        	canEnc[0].cycle();
        	canEnc[1].cycle();

        	canPulse[0].cycle();
        	canPulse[1].cycle();
        	canPulse[2].cycle();
        	canPulse[3].cycle();
    	}

    	if(canNodeMode == CAN_NODE_MODE_VALVE){
        	canEnc[0].cycle();
        	canEnc[1].cycle();
        	canEnc[2].cycle();
        	canEnc[3].cycle();
    	}


    	if(printTime <= millis()){
    		printTime = millis() + PRINT_TIME;

    		if(canLastReceiveTime + 50 < millis()){
    			led.toggle();
    		}

        	if(sw.read() == 0){
        		if(switchStat == 0){
        			switchStat = 1;
        			printMode++;
        		}
        	}else{
        		switchStat = 0;
        	}

        	if(canNodeMode == CAN_NODE_MODE_AMP){
        		switch(printMode){
           		case 0:
           			serial.printf("duty=%4d,%4d\n\r",canPulse[0].outDuty,canPulse[1].outDuty);
           			break;
           		case 1:
           			serial.printf("itv=%4d,cnt=%6d,itv=%4d,cnt=%4d\n\r",canEnc[0].canEnc_interval,(int32_t)canEnc[0].canEnc_enc->read(),canEnc[1].canEnc_interval,(int32_t)canEnc[1].canEnc_enc->read());
           			break;
           		case 2:
           			//CANŽóM
           			break;
           		default:
           			printMode = 0;
           			break;
           		}
        	}

        	if(canNodeMode == CAN_NODE_MODE_VALVE){
           		switch(printMode){
           		case 0:
           			serial.printf("itv = %2d,cnt = %8d, itv = %2d,cnt = %8d, itv = %2d,cnt = %8d, itv = %2d,cnt = %8d\n\r",(uint32_t)canEnc[0].canEnc_interval,(uint32_t)enc[0].read(),(uint32_t)canEnc[1].canEnc_interval,(uint32_t)enc[1].read(),(uint32_t)canEnc[2].canEnc_interval,(uint32_t)enc[2].read(),(uint32_t)canEnc[3].canEnc_interval,(uint32_t)enc[3].read());
           			break;
           		case 1:
           			serial.printf("ADD = %x,0 = %d,1 = %d\n\r",can1.rxMessage.StdId,can1.rxMessage.Data[0],can1.rxMessage.Data[1]);
           			break;
           		case 2:
           			//CANŽóM
           			break;
           		case 3:

           			break;
           		case 4:

           			break;
           		default:
           			printMode = 0;
           			break;
           		}
        	}
    	}

    	while(rxFlag > 0){
    		rxFlag--;
    		led.toggle();
    		if(printMode == 2)serial.printf("ADD = %x,%d\n\r",can1.rxMessage.StdId,can1.rxMessage.Data[1]);
    	}
    }
}
