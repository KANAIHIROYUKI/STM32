#ifndef SPI_H_
#define SPI_H_

#include "stm32f0xx.h"
#include "stdio.h"


#define SPI_Mode0 0
#define SPI_Mode1 1
#define SPI_Mode2 2
#define SPI_Mode3 3

#define SPI_PSS_LP 0//PositiveSlaveSelect/LatchPreceding
#define SPI_PSS_SP 1
#define SPI_NSS_LP 2
#define SPI_NSS_SP 3//NegativeSlaveSelect/ShiftPreceding


void SPI1Setup(uint16_t SPI_Mode,uint16_t SPI_Protocol_Mode,uint16_t SPI_Prescaler);

void SPI2Setup(uint16_t SPI_Mode,uint16_t SPI_Protocol_Mode,uint16_t SPI_Prescaler);



#endif
