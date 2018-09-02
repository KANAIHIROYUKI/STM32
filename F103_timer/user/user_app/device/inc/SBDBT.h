#ifndef SBDBT_H_
#define SBDBT_H_

#include "system.h"
#include "USART.h"

#define DataBufferSize 18

#define PS3_UP		0
#define PS3_DOWN	1
#define PS3_RIGHT	2
#define PS3_LEFT	3
#define PS3_TRI		4
#define PS3_CRO		5
#define PS3_CIR		6
#define PS3_SQR		7
#define PS3_L1		8
#define PS3_L2		9
#define PS3_R1		10
#define PS3_R2		11
#define PS3_Start	12
#define PS3_Select	13
#define PS3_HatLX	14
#define PS3_HatLY	15
#define PS3_HatRX	16
#define PS3_HatRY	17


class SBDBT {
public:
	void setup(USART_TypeDef *usart,GPIO_TypeDef* gpio_tx,uint16_t pin_tx,GPIO_TypeDef* gpio_rx,uint16_t pin_rx,uint32_t baud);

	void cycle();

	int16_t read(uint16_t address);

	int16_t dataBuf[DataBufferSize];
	int16_t data[DataBufferSize];
	int16_t oldData[DataBufferSize];

	uint16_t rxNum,parity;	//éÛêMån

	uint16_t update,error;
	uint64_t lastReceiveTime;
	USART serial;
private:
};


#endif
