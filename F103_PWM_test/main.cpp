#include "main.hpp"

int main(void)
{
	setup();
	uint32_t i = 0,num = 0;
	serial.printf("%x,%x,%x",FLASH_BUSY,FLASH_ERROR_WRP,FLASH_TIMEOUT);
	for(int n = 0;n<256;n++){
		serial.printf("%x,%x\n\r",n,FlashRead(n));
		FlashWrite(n,0xFF000000 + n);
		delay(10);
	}
	sys.wdgSetup(1000);
    while(1){
    	sys.cycle();
    	if(intervalTime < millis()){
    		intervalTime += 50;
    		//serial.printf("%d,\n\r",adc.read()*4);
    		pwm.duty(adc.read()*4);
    		serial.printf("i = %d,%x\n\r",i,FlashRead(i));

    		i++;
    		if(i > 255)i = 0;
    	}
    	if(serial.available()){
    		if(serial.peek() == 'r'){
    			while(1);
    		}else if(serial.peek() == 'm'){
    			uint64_t time = millis();
    			serial.printf("flash = %x,",FlashErase());
    			serial.printf("erase time = %d\n\r",millis() - time);
    		}else if(serial.peek() == 'c'){
    			serial.printf("ebusy = %d\n\r",FLASH_GetFlagStatus(FLASH_BUSY));
    			serial.printf("eop = %d\n\r",FLASH_GetFlagStatus(FLASH_FLAG_EOP));
    			serial.printf("pg = %d\n\r",FLASH_GetFlagStatus(FLASH_ERROR_PG));
    			serial.printf("wrp = %d\n\r",FLASH_GetFlagStatus(FLASH_ERROR_WRP));
    			serial.printf("opterr = %d\n\r",FLASH_GetFlagStatus(FLASH_FLAG_OPTERR));
    		}else{

    			FlashWrite(num,0xFF000000 + num);
    			num++;
    			serial.printf("data = %x\n\r",num);
    			if(num > 255)num = 0;
    		}
    		while(serial.available())serial.read();
    	}
    }
}


uint32_t FlashRead(uint32_t address){	//F103‚Í1ƒy[ƒW1KB‚È‚Ì‚Å
	if(address > 255)return 0;
	return *(uint32_t *)(0x801FC00 + address * 4);
}
void FlashWrite(uint32_t address,uint32_t data){

	if(address > 255)return;

	while(FLASH_GetFlagStatus(FLASH_BUSY) == SET);

	FLASH_Unlock();
	FLASH_ProgramWord(0x801FC00 + address*4,data);
	FLASH_Lock();
}

int FlashErase(){
	FLASH_Unlock();
	FLASH_ErasePage(0x801FC00);
	while(FLASH_GetFlagStatus(FLASH_BUSY) == SET);
	FLASH_Lock();
}
