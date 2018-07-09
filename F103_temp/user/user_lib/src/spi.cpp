#include "spi.h"

uint16_t SPI_Master::interruptCnt = 0,SPI_Master::writeCnt = 0;;

RingBuffer<uint8_t,SPI_BufferSize> SPI_Master::spi1txBuffer;
RingBuffer<uint8_t,SPI_BufferSize> SPI_Master::spi2txBuffer;
RingBuffer<uint8_t,SPI_BufferSize> SPI_Master::spi1rxBuffer;
RingBuffer<uint8_t,SPI_BufferSize> SPI_Master::spi2rxBuffer;

void SPI_Master::setup(SPI_TypeDef *spiSet
		,GPIO_TypeDef *sck,uint16_t sckPin
		,GPIO_TypeDef *miso,uint16_t misoPin
		,GPIO_TypeDef *mosi,uint16_t mosiPin
		,uint16_t modeSet,uint16_t prescalerSet){
	spi = spiSet;
	mode = modeSet;
	prescaler = prescalerSet;
	ioSetup(sck,sckPin,miso,misoPin,mosi,mosiPin);

	if(spi == SPI1){
		SPI1Setup(SPI_Mode_Master,mode,prescaler);
	}else if(spi == SPI2){
		SPI2Setup(SPI_Mode_Master,mode,prescaler);
	}else{
		return;
	}

	interfaceNumberCnt = 0;
	takeInterfaceNumber = 0;
}

uint16_t SPI_Master::getInterfaceNumber(){
	interfaceNumberCnt++;
	return interfaceNumberCnt;
}

uint16_t SPI_Master::take(uint16_t interfaceNumber){
	if(takeInterfaceNumber != 0)return 0;				//どれかinterfaceクラスが使用中
	if(takeInterfaceNumber == interfaceNumber)return 0;	//既に使っとるわ！
	takeInterfaceNumber = interfaceNumber;

	if(spi == SPI1){
		spi1rxBuffer.clear();
		spi1txBuffer.clear();
	}else if(spi == SPI2){
		spi2rxBuffer.clear();
		spi2txBuffer.clear();
	}

	return 1;
}

void SPI_Master::releace(uint16_t interfaceNumber){
	if(interfaceNumber == takeInterfaceNumber)takeInterfaceNumber = 0;
}

void SPI_Master::ioSetup(GPIO_TypeDef *sck,uint16_t sckPin,GPIO_TypeDef *miso,uint16_t misoPin,GPIO_TypeDef *mosi,uint16_t mosiPin){
	GPIO MOSI,MISO,SCK;

	MOSI.setup(mosi,mosiPin,OUTPUT_AF);
	SCK.setup(sck,sckPin,OUTPUT_AF);
	MISO.setup(miso,misoPin,INPUT);
}


uint8_t SPI_Master::transfer(uint8_t data){
	if(spi == SPI1){
		SPI_I2S_SendData(SPI1,data);
		while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE) == RESET);
		return SPI_I2S_ReceiveData(SPI1);

	}else if(spi == SPI2){
		SPI_I2S_SendData(SPI2,data);
		while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_RXNE) == RESET);
		return SPI_I2S_ReceiveData(SPI2);

	}else{
		return 0;
	}
}

void SPI_Master::write(uint8_t data){
	if(spi == SPI1){
		spi1txBuffer.write(data);
	}else if(spi == SPI2){
		spi2txBuffer.write(data);
	}else{
		return;
	}
}

void SPI_Master::send(){
	if(spi == SPI1){
		uint8_t data = SPI_Master::interruptCnt;
		SPI_Cmd(SPI1, ENABLE);
		SPI_I2S_SendData(SPI1,data);
		SPI_I2S_ITConfig(SPI1,SPI_I2S_FLAG_TXE,ENABLE);
	}else if(spi == SPI2){
		SPI_I2S_ITConfig(SPI2,SPI_I2S_FLAG_TXE,ENABLE);
		SPI_I2S_SendData(SPI2,0x8);
		while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE) == SET);
		SPI_I2S_SendData(SPI2,0x81);
	}
}

uint8_t SPI_Master::read(){
	if(spi == SPI1){
		return spi1rxBuffer.read();
	}else if(spi == SPI2){
		return spi2rxBuffer.read();
	}else{
		return 0;
	}
}

uint16_t SPI_Master::txLength(){
	if(spi == SPI1){
		return spi1txBuffer.length();
	}else if(spi == SPI2){
		return spi2txBuffer.length();
	}else{
		return 0;
	}
}

uint16_t SPI_Master::available(){
	if(spi == SPI1){
		return spi1rxBuffer.length();
	}else if(spi == SPI2){
		return spi2rxBuffer.length();
	}else{
		return 0;
	}
}

uint16_t SPI_Master::rxEmpty(){
	if(spi == SPI1){
		return spi1rxBuffer.isEmpty();
	}else if(spi == SPI2){
		return spi2rxBuffer.isEmpty();
	}else{
		return 0;
	}
}

uint16_t SPI_Master::txEmpty(){
	if(spi == SPI1){
		return spi1txBuffer.isEmpty();
	}else if(spi == SPI2){
		return spi2txBuffer.isEmpty();
	}else{
		return 0;
	}
}

/********************************************************************************/
/*
void SPI_Slave::setup(SPI_TypeDef *spi
		,GPIO_TypeDef *sck,uint16_t sckPin
		,GPIO_TypeDef *miso,uint16_t misoPin
		,GPIO_TypeDef *mosi,uint16_t mosiPin
		,uint16_t modeSet){
	mode = modeSet;
	ioSetup(sck,sckPin,miso,misoPin,mosi,mosiPin);


	if(spi == SPI1){
		SPI1Setup(SPI_Mode_Slave,mode);
	}else{
		return;
	}
}

void SPI_Slave::ioSetup(GPIO_TypeDef *sck,uint16_t sckPin,GPIO_TypeDef *miso,uint16_t misoPin,GPIO_TypeDef *mosi,uint16_t mosiPin){
	GPIO MOSI,MISO,SCK;

	MOSI.setup(mosi,mosiPin,INPUT_PU);
	SCK.setup(sck,sckPin,INPUT_PU);
	MISO.setup(miso,misoPin,OUTPUT_AF);
}

*/



void SPI1Setup(uint16_t SPI_Mode,uint16_t SPI_Protocol_Mode,uint16_t SPI_Prescaler){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

	SPI_InitTypeDef SPI_InitStructure;

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;

	switch(SPI_Protocol_Mode){
	case SPI_Mode0:
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
		break;
	case SPI_Mode1:
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
		break;
	case SPI_Mode2:
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
		break;
	case SPI_Mode3:
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
		break;
	default://mode3　後･上昇端読み込み
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
		break;
	}


	if(SPI_Mode == SPI_Mode_Master){
		SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;// | SPI_NSSInternalSoft_Set;
		//SPI_NSSInternalSoftwareConfig(SPI1,SPI_NSSInternalSoft_Set);

		SPI_InitStructure.SPI_BaudRatePrescaler = SPI_Prescaler;
	}else{
		SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;//内部NSSとNSSピンを接続↓
	}

	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	//SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &SPI_InitStructure);


	//割り込み設定
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = SPI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	SPI_I2S_ITConfig(SPI1,SPI_I2S_IT_TXE,ENABLE);
	SPI_I2S_ITConfig(SPI1,SPI_I2S_IT_RXNE,ENABLE);

	//SPI有効
	SPI_Cmd(SPI1, ENABLE);
}


void SPI2Setup(uint16_t SPI_Mode,uint16_t SPI_Protocol_Mode,uint16_t SPI_Prescaler){		//SPI1のコピペ｡あやしい
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

	SPI_InitTypeDef SPI_InitStructure;

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;

	//SPI_Protocol_Mode = SPI_Mode0;
	switch(SPI_Protocol_Mode){
	case SPI_Mode0:
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
		break;
	case SPI_Mode1:
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
		break;
	case SPI_Mode2:
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
		break;
	case SPI_Mode3:
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
		break;
	default://mode3　後･上昇端読み込み
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
		break;
	}


	if(SPI_Mode == SPI_Mode_Master){
		SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;// | SPI_NSSInternalSoft_Set;
		//SPI_NSSInternalSoftwareConfig(SPI2,SPI_NSSInternalSoft_Set);

		SPI_InitStructure.SPI_BaudRatePrescaler = SPI_Prescaler;
	}else{
		SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;//内部NSSとNSSピンを接続↓
	}

	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	//SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI2, &SPI_InitStructure);


	//割り込み設定
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = SPI2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	//SPI_I2S_ITConfig(SPI2,SPI_I2S_IT_TXE,ENABLE);
	//SPI_I2S_ITConfig(SPI2,SPI_I2S_IT_RXNE,ENABLE);

	//SPI有効
	SPI_Cmd(SPI2, ENABLE);
}

extern "C" void SPI1_IRQHandler(){
	SPI_Master::interruptCnt++;
	if(SPI_I2S_GetITStatus(SPI1,SPI_I2S_IT_TXE) == SET){

		if(SPI_Master::spi1txBuffer.isEmpty()){
			//SPI_I2S_ITConfig(SPI1,SPI_I2S_IT_TXE,DISABLE);
			SPI_I2S_ClearITPendingBit(SPI1,SPI_I2S_FLAG_TXE);
			//SPI_I2S_SendData(SPI1,0xC3);
			SPI_Cmd(SPI1, DISABLE);
		}else{
			SPI_Master::spi1txBuffer.read();
			SPI_I2S_SendData(SPI1,SPI_Master::interruptCnt);
			//SPI_I2S_SendData(SPI1,SPI_Master::spi1txBuffer.read());
		}
	}

	if(SPI_I2S_GetITStatus(SPI1,SPI_I2S_IT_RXNE)){
		SPI_I2S_ClearITPendingBit(SPI1,SPI_I2S_FLAG_RXNE);
		SPI_Master::spi1rxBuffer.write(SPI_I2S_ReceiveData(SPI1));
	}
}


extern "C" void SPI2_IRQHandler(){
	SPI_Master::interruptCnt++;

	if(SPI_I2S_GetITStatus(SPI2,SPI_I2S_IT_TXE) == SET){
		SPI_I2S_ClearITPendingBit(SPI2,SPI_I2S_FLAG_TXE);
		if(SPI_Master::spi2txBuffer.length() == 0){
			//SPI_I2S_ITConfig(SPI2,SPI_I2S_IT_TXE,DISABLE);

			SPI_I2S_SendData(SPI2,0xAA);
		}else{
			SPI_Master::spi2txBuffer.read();
			SPI_I2S_SendData(SPI2,0xC3);
		}
	}

	if(SPI_I2S_GetITStatus(SPI2,SPI_I2S_IT_RXNE)){
		SPI_I2S_ClearITPendingBit(SPI2,SPI_I2S_FLAG_RXNE);
		SPI_Master::spi2rxBuffer.write(SPI_I2S_ReceiveData(SPI2));
	}
}
