#include "main.hpp"

int main(void)
{
	setup();

	while(1){
		ss.write(0);
	    uint8_t data1 = spi.transfer(0xaf);
	    uint8_t data2 = spi.transfer(0xaf);
	    ss.write(1);
	    uint32_t num = ((data1 << 3) + (data2 >> 5)) & 0x3FF;

	    serial.printf("%d,\n\r",num);
	    delay(100);
	}

	while(1){
		ss.write(0);
		for(int i=0;i<10;i++){
			spi.write(i);
		}
		spi.send();
		delay(1);
		ss.write(1);
		delay(100);
		serial.printf("tx buf = %d,int = %d,",spi.spi1txBuffer.length(),spi.interruptCnt);
		serial.printf("rx = ");
		while(spi.available()){
			serial.printf("%d,",spi.read());
		}
		serial.printf("\n\r");
		//spi.interruptCnt = 0;
	}

	uint32_t i = 0,num = 0;
	serial.printf("%x,%x,%x",FLASH_BUSY,FLASH_ERROR_WRP,FLASH_TIMEOUT);
	for(int n = 0;n<256;n++){
		serial.printf("%x,%x\n\r",n,FlashRead(n));
		FlashWrite(n,0xFF000000 + n);
		delay(10);
	}
	sys.wdgSetup(1000);
    while(1){

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
