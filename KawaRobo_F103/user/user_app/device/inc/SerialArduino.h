#ifndef SerialArduino_H_
#define SerialArduino_H_

#include "system.h"
#include "USART.h"

#define SA_FRAME_START	0b00
//+���M�o�C�g��

#define SA_FRAME_DATA	0b01
#define SA_FRAME_END	0b10

#define SA_DATA_SIZE 12
//16bit�^�ő��M�ł���f�[�^������f�[�^�����o�C�g��

/*
 * | FrameType | FrameType | Data5 | Data4 | Data3 | Data3 | Data2 | Data1 | Data0
 */

class SerialArduino {
public:
	void setup(USART_TypeDef *usart,uint32_t baud,GPIO_TypeDef* gpio_tx,uint16_t pin_tx,GPIO_TypeDef* gpio_rx,uint16_t pin_rx);

	void cycle();

	void write(uint16_t address,int16_t data);
	int16_t read(uint16_t address);

	int16_t data[SA_DATA_SIZE];

	uint16_t bufferAddress,frameSize;	//��M�n

	uint8_t buffer[SA_DATA_SIZE*3];
	int8_t sendBuffer[SA_DATA_SIZE*3];
	uint16_t receiveStat,sendRequest;	//���M�n
	uint16_t update;
	uint64_t lastReceiveTime;
	USART serial;
private:
};


#endif
