#ifndef SPI_H_
#define SPI_H_

#include "stm32f10x.h"
#include <stdio.h>

#include "systime.h"
#include "gpio.h"
#include "util.hpp"

#define SPI_Mode0 (uint16_t)0
#define SPI_Mode1 (uint16_t)1
#define SPI_Mode2 (uint16_t)2
#define SPI_Mode3 (uint16_t)3

#define SPI_PSS_LP 0//PositiveSlaveSelect/LatchPreceding
#define SPI_PSS_SP 1
#define SPI_NSS_LP 2
#define SPI_NSS_SP 3//NegativeSlaveSelect/ShiftPreceding

#define SPI_BufferSize 128


class SPI_Master{
public:
	void setup(SPI_TypeDef *spi
			,GPIO_TypeDef *sck,uint16_t sckPin
			,GPIO_TypeDef *miso,uint16_t misoPin
			,GPIO_TypeDef *mosi,uint16_t mosiPin
			,uint16_t mode = SPI_Mode3,uint16_t prescaler = SPI_BaudRatePrescaler_256);

	uint16_t getInterfaceNumber();
	uint16_t take(uint16_t takeInterfaceNumber);
	void releace(uint16_t takeInterfaceNumber);

	uint16_t mode,prescaler;

	void ioSetup(GPIO_TypeDef *sck,uint16_t sckPin,GPIO_TypeDef *miso,uint16_t misoPin,GPIO_TypeDef *mosi,uint16_t mosiPin);

	void cycle();

	uint8_t transfer(uint8_t data);

	void write(uint8_t data);
	void send();
	uint8_t read();
	uint16_t available();
	uint16_t txLength();

	uint16_t rxEmpty();
	uint16_t txEmpty();


	static void spi1TransferInterrupt();
	static void spi1ReceiveInterrupt();
	static void spi2TransferInterrupt();
	static void spi2ReceiveInterrupt();

	static uint16_t interruptCnt,writeCnt;
	uint16_t interfaceNumberCnt,takeInterfaceNumber;
	static RingBuffer<uint8_t,SPI_BufferSize> spi1txBuffer,spi2txBuffer,spi1rxBuffer,spi2rxBuffer;
	SPI_TypeDef *spi;
};


/*
class SPI_Slave{
public:
	void setup(SPI_TypeDef *spi
			,GPIO_TypeDef *sck,uint16_t sckPin
			,GPIO_TypeDef *miso,uint16_t misoPin
			,GPIO_TypeDef *mosi,uint16_t mosiPin
			,uint16_t mode = SPI_Mode3);

	uint16_t getInterfaceNumber();

	uint16_t mode;

	void ioSetup(GPIO_TypeDef *sck,uint16_t sckPin,GPIO_TypeDef *miso,uint16_t misoPin,GPIO_TypeDef *mosi,uint16_t mosiPin);

	static uint16_t interfaceNumber,takeInterfaceNumber;
};
*/

void SPI1Setup(uint16_t SPI_Mode,uint16_t SPI_Protocol_Mode = SPI_Mode3,uint16_t SPI_Prescaler = SPI_BaudRatePrescaler_256);
void SPI2Setup(uint16_t SPI_Mode,uint16_t SPI_Protocol_Mode = SPI_Mode3,uint16_t SPI_Prescaler = SPI_BaudRatePrescaler_256);

#endif
