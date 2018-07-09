#include "main.hpp"

int main(void)
{
	setup();
/*
	while(1){
		for(int i=0;i<10;i++){
			spi.write(i + 10);
		}
		ss.write(0);
		spi.send();
		delay(1);
		ss.write(1);

		delay(100);
		serial.printf("tx buf = %d,int = %d,",spi.txLength(),spi.interruptCnt);
		serial.printf("rx = ");
		while(spi.available()){
			serial.printf("%d,",spi.read());
		}
		serial.printf("\n\r");
		//spi.interruptCnt = 0;
	}*/
	while(1){

		ps3.serial.send(0x80);

		ps3.serial.send(0x7F);
		ps3.serial.send(0x7F);

		ps3.serial.send(0x3F);
		ps3.serial.send(0x3E);
		ps3.serial.send(0x3D);
		ps3.serial.send(0x3C);

		ps3.serial.send(0x74);

		ps3.cycle();

		for(int i=0;i<18;i++){
			serial.printf("%d,",ps3.read(i));
		}
		serial.printf("p = %x,\n\r",ps3.parity);
		serial.printf("up = %d,er = %d\n\r",ps3.update,ps3.error);

		ps3.update = 0;
		delay(100);
		led[0].cycle();
		led[1].cycle();
		led[2].cycle();
		while(serial.available());
	}


	serial.printf("%x,%x,%x",FLASH_BUSY,FLASH_ERROR_WRP,FLASH_TIMEOUT);
	for(int n = 0;n<64;n++){
		serial.printf("%x,%x\n\r",n,sys.flashRead(n));
		sys.flashWrite(n,0xFF000000 + n);
		delay(10);
	}
	//sys.wdgSetup(1000);
    while(1){
    	sys.cycle();
    	ps3.cycle();
    	enc.cycle();

    	if(serial.available()){
    		switch(serial.read()){
    		case 'w':
    			serial.printf("write mode\n\r input number");
    			while(serial.available() == 0);
    			serial.printf("num = %d\n\r",serial.peek());
    			for(int n = 0;n<64;n++){
    				sys.flashWrite(n,0xFF000000 + n + serial.peek());
    				delay(10);
    			}
    			serial.read();
    			break;
    		case 'r':
    			serial.printf("read mode\n\r");
    			for(int n = 0;n<16;n++){
    				serial.printf("%x,%x\n\r",n,sys.flashRead(n));
    			}
    			break;
    		case 'e':
    			serial.printf("elease mode\n\r");
    			sys.flashErase();
    			break;
    		}
    		delay(50);
    		serial.flush();
    	}
    }
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
