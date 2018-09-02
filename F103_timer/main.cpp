#include "main.hpp"

int main(void)
{
	setup();

	textCnt = 0;
	commaCnt = 0;
	hoge = 0;

	intervalTime = 0;

    while(1){
    	sys.cycle();

    	if(intervalTime < millis()){
    		intervalTime += 1000;
    		//serial.printf("hoge = %d,\n\r",hoge);

    		//serial.printf("\n\r");
    		for(int i=0;i<8;i++){
    			//serial.send(outputText[i]);
    		}
    		//serial.printf("\n\r");

    	}

		hoge++;
		if(hoge > 8)hoge = 0;
		vfd(hoge,outputText[hoge] - '0');

    	if(gps.available()){
    		serial.send(gps.peek());

    		switch(textCnt){
    		case 0:
    			break;
    		case 1:
    			if(gps.peek() == 'G'){
    	    		//serial.printf("num = %d\n\r",textCnt);
    				textCnt++;
    			}else{
    				textCnt = 0;
    			}
    			break;
    		case 2:
    			if(gps.peek() == 'P'){
    	    		//serial.printf("num = %d\n\r",textCnt);
    				textCnt++;
    			}else{
    				textCnt = 0;
    			}
    			break;
    		case 3:
    			if(gps.peek() == 'R'){
    	    		//serial.printf("num = %d\n\r",textCnt);
    				textCnt++;
    			}else{
    				textCnt = 0;
    			}
    			break;
    		case 4:
    			if(gps.peek() == 'M'){
    	    		//serial.printf("num = %d\n\r",textCnt);
    				textCnt++;
    			}else{
    				textCnt = 0;
    			}
    			break;
    		case 5:
    			if(gps.peek() == 'C'){
    	    		//serial.printf("num = %d\n\r",textCnt);
    				textCnt++;
    			}else{
    				textCnt = 0;
    			}
    			break;
    		case 6:
    			if(gps.peek() == ','){
    				textCnt++;
    				commaCnt = 0;
    				//serial.printf("\n\r\n\r detect \n\r\n\r");
    			}else{
    				textCnt = 0;
    			}
    			break;
    		default:
        		if(textCnt > 14 && commaCnt >= 8){
        			textCnt = 0;

        			for(int i=0;i<10;i++){
        				outputText[i] = outputTextBuf[i];
        			}

        			int num = (outputTextBuf[3] - '0') * 10 + (outputTextBuf[4] - '0') + 9;
        			if(num >= 24){
        				num-=24;
        				int day = (outputTextBuf[0] - '0') * 10 + (outputTextBuf[1] - '0') + 1;
        				if(day > 31)day = 1;
        				outputText[1] = '0' + day%10;
        				outputText[0] = '0' + (day - day%10)/10;
        			}

        			outputText[4] = '0' + num%10;
        			outputText[3] = '0' + (num - num%10)/10;
        		}else if(commaCnt == 0 && textCnt < 13){
        			textCnt++;
        			outputTextBuf[textCnt - 5] = gps.peek();
        			//serial.send(gps.peek());
        		}else{
        			if(gps.peek() == ','){
        				commaCnt++;
        			}else if(commaCnt == 8){
        				textCnt++;
        				outputTextBuf[textCnt - 14] = gps.peek();
        			}else if(serial.peek() == 'N'){
        				serial.send('*');
        				textCnt = 0;
        				commaCnt = 0;
        			}
        		}
    		}

    		if(gps.peek() == '$')textCnt = 1;
    		gps.read();
    	}
    }
}

void vfd(uint16_t col,uint16_t num){
	grid[col].write(1);
	switch(num){
	case 0:
		segment[SEG_a].write(1);
		segment[SEG_b].write(1);
		segment[SEG_c].write(1);
		segment[SEG_d].write(1);
		segment[SEG_e].write(1);
		segment[SEG_f].write(1);
		break;
	case 1:
		segment[SEG_c].write(1);
		segment[SEG_b].write(1);
		break;
	case 2:
		segment[SEG_a].write(1);
		segment[SEG_b].write(1);
		segment[SEG_g].write(1);
		segment[SEG_e].write(1);
		segment[SEG_d].write(1);
		break;
	case 3:
		segment[SEG_a].write(1);
		segment[SEG_b].write(1);
		segment[SEG_g].write(1);
		segment[SEG_c].write(1);
		segment[SEG_d].write(1);
		break;
	case 4:
		segment[SEG_f].write(1);
		segment[SEG_b].write(1);
		segment[SEG_g].write(1);
		segment[SEG_c].write(1);
		break;
	case 5:
		segment[SEG_a].write(1);
		segment[SEG_f].write(1);
		segment[SEG_g].write(1);
		segment[SEG_c].write(1);
		segment[SEG_d].write(1);
		break;
	case 6:
		segment[SEG_a].write(1);
		segment[SEG_c].write(1);
		segment[SEG_d].write(1);
		segment[SEG_e].write(1);
		segment[SEG_f].write(1);
		segment[SEG_g].write(1);
		break;
	case 7:
		segment[SEG_a].write(1);
		segment[SEG_b].write(1);
		segment[SEG_c].write(1);
		segment[SEG_f].write(1);
		break;
	case 8:
		segment[SEG_a].write(1);
		segment[SEG_b].write(1);
		segment[SEG_c].write(1);
		segment[SEG_d].write(1);
		segment[SEG_e].write(1);
		segment[SEG_f].write(1);
		segment[SEG_g].write(1);
		break;
	case 9:
		segment[SEG_a].write(1);
		segment[SEG_b].write(1);
		segment[SEG_c].write(1);
		segment[SEG_d].write(1);
		segment[SEG_f].write(1);
		segment[SEG_g].write(1);
		break;
	}
	delayMicros(200);
	for(int i=0;i<11;i++){
		segment[i].write(0);
	}
	grid[col].write(0);
	delayMicros(100);
}

/*
uint32_t FlashRead(uint32_t address){	//F103‚Í1ƒy[ƒW1KB‚È‚Ì‚Å
	if(address > 255)return 0;
	return *(uint32_t *)(0x801FC00 + address * 4);
}
void FlashWrite(uint32_t address,uint32_t data){

	if(address > 255)return;

	uint64_t startTime = millis();
	while(FLASH_GetFlagStatus(FLASH_BUSY) == SET){
		if(millis() - startTime > 100){
			return 1;
		}
	}

	FLASH_Unlock();
	FLASH_ProgramWord(0x801FC00 + address*4,data);
	FLASH_Lock();
}

bool FlashErase(){
	FLASH_Unlock();
	FLASH_ErasePage(0x801FC00);
	uint64_t startTime = millis();
	while(FLASH_GetFlagStatus(FLASH_BUSY) == SET){
		if(millis() - startTime > 100){
			return 1;
		}
	}
	FLASH_Lock();
	return 0;
}
*/
