#include "spi.h"


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
		SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;// | SPI_NSSInternalSoft_Set;

		if(IS_SPI_BAUDRATE_PRESCALER(SPI_Prescaler)){
			SPI_InitStructure.SPI_BaudRatePrescaler = SPI_Prescaler;
		}else{
			SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
		}
	}else{
		SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;//内部NSSとNSSピンを接続↓
	}

	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	//SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &SPI_InitStructure);


	//割り込み設定
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = SPI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	if(SPI_Mode == SPI_Mode_Slave){
		SPI_I2S_ITConfig(SPI1,SPI_I2S_IT_RXNE,ENABLE);
		SPI_I2S_ITConfig(SPI1,SPI_I2S_IT_TXE,ENABLE);
	}

	//SPI有効
	SPI_Cmd(SPI1, ENABLE);

}

extern "C" void SPI1_IRQHandler(){
	if(SPI_I2S_GetITStatus(SPI1,SPI_I2S_IT_TXE)){
		SPI_I2S_ClearITPendingBit(SPI1,SPI_I2S_FLAG_TXE);

	}else if(SPI_I2S_GetITStatus(SPI1,SPI_I2S_IT_RXNE)){
		SPI_I2S_ClearITPendingBit(SPI1,SPI_I2S_FLAG_RXNE);

	}else{

	}
}
