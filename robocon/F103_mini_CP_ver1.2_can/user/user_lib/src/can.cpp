#include "can.h"

uint16_t CAN::filterCnt = 0;
uint16_t CAN::filterAddress[4*13],CAN::errorCnt = 0,CAN::errorAddress = 0,CAN::sendAddress = 0;

void CAN::setup(CAN_TypeDef* can,GPIO_TypeDef* gpio_tx,uint16_t pin_tx,GPIO_TypeDef* gpio_rx,uint16_t pin_rx){
	can_can = can;
	ioSetup(gpio_tx,pin_tx,gpio_rx,pin_rx);

	if(can_can == CAN1){
		CAN1Setup();
	}else{

	}

	for(int i=0;i<4*13;i++){
		filterAddress[i] = 0;
	}
}

void CAN::send(uint16_t id,uint8_t length,uint8_t *data){
	if(can_can == CAN1){
		CAN1Send(id,length,data);
	}else{

	}
}

void CAN::filterAdd(uint16_t id1,uint16_t id2,uint16_t id3,uint16_t id4){
	if(id1 != 0){
		filterAddress[filterCnt] = id1;
		filterCnt++;
	}
	if(id2 != 0){
		filterAddress[filterCnt] = id2;
		filterCnt++;
	}
	if(id3 != 0){
		filterAddress[filterCnt] = id3;
		filterCnt++;
	}
	if(id4 != 0){
		filterAddress[filterCnt] = id4;
		filterCnt++;
	}

	CANFilterAdd(id1,id2,id3,id4);
}

void CAN::receive(CanRxMsg* canRxMessage){
	if(can_can == CAN1){
		CAN_Receive(CAN1,CAN_FIFO0,canRxMessage);
	}else{

	}
}

void CAN::ioSetup(GPIO_TypeDef* gpio_tx,uint16_t pin_tx,GPIO_TypeDef* gpio_rx,uint16_t pin_rx){
	GPIO RX,TX;
	TX.setup(gpio_tx,pin_tx,OUTPUT_AF);
	RX.setup(gpio_rx,pin_rx,INPUT);


}

void CAN::receive(){
	if(can_can == CAN1){
		CAN_Receive(CAN1,CAN_FIFO0,&rxMessage);
	}else{

	}
}

void CAN1Setup(){
	CAN_DeInit(CAN1);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1,ENABLE);

	CAN_InitTypeDef CAN_InitStructure;
	CAN_StructInit(&CAN_InitStructure);
	CAN_InitStructure.CAN_TTCM = DISABLE;
	CAN_InitStructure.CAN_ABOM = DISABLE;
	CAN_InitStructure.CAN_AWUM = DISABLE;
	CAN_InitStructure.CAN_NART = DISABLE;
	CAN_InitStructure.CAN_TXFP = DISABLE;
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
	//CAN_InitStructure.CAN_Mode = CAN_Mode_LoopBack;
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
	CAN_InitStructure.CAN_BS1 = CAN_BS1_5tq;
	CAN_InitStructure.CAN_BS2 = CAN_BS2_6tq;
	CAN_InitStructure.CAN_Prescaler = 3;
	CAN_Init(CAN1,&CAN_InitStructure);

/*
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
    CAN_FilterInitStructure.CAN_FilterNumber = 0;
    CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList;
    CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_16bit;
    CAN_FilterInitStructure.CAN_FilterIdLow = (0x400+0)<<5;
    CAN_FilterInitStructure.CAN_FilterMaskIdLow = (0x400+1)<<5;
    CAN_FilterInitStructure.CAN_FilterIdHigh = (0x280)<<5;
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh = (0x280+1)<<5;
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
    CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
    CAN_FilterInit(&CAN_FilterInitStructure);*/


    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);	//FIFO0ŽóMŠ„ž‚Ý
}

void CAN1Send(uint16_t id,uint8_t length,uint8_t data[8]){
	uint64_t timeOut = millis();

    while(CANTXOK != CAN_TransmitStatus(CAN1,0)){
    	if(millis() - timeOut > 10){
    		CAN::errorCnt++;
    		CAN::errorAddress = CAN::sendAddress;
    		break;
    	}
    }

    CAN::sendAddress = id;

    CanTxMsg txMessage;
    txMessage.StdId = id;
    txMessage.IDE   = CAN_ID_STD;
    if(length == 9){
    	txMessage.RTR = CAN_RTR_REMOTE;
    	length = 8;
    }else{
    	txMessage.RTR   = CAN_RTR_DATA;
    }
    txMessage.DLC   = length;
    txMessage.Data[0] = data[0];
    txMessage.Data[1] = data[1];
    txMessage.Data[2] = data[2];
    txMessage.Data[3] = data[3];
    txMessage.Data[4] = data[4];
    txMessage.Data[5] = data[5];
    txMessage.Data[6] = data[6];
    txMessage.Data[7] = data[7];

    CAN_Transmit(CAN1,&txMessage);

}

void CANFilterAdd(uint16_t id1,uint16_t id2,uint16_t id3,uint16_t id4){
	static uint16_t filterCnt = 0;

	if(filterCnt > 13)return;

	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
    CAN_FilterInitStructure.CAN_FilterNumber = filterCnt;
    CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList;
    CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_16bit;
    CAN_FilterInitStructure.CAN_FilterIdLow = id1<<5;
    CAN_FilterInitStructure.CAN_FilterMaskIdLow = id2<<5;
    CAN_FilterInitStructure.CAN_FilterIdHigh = id3<<5;
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh = id4<<5;
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
    CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
    CAN_FilterInit(&CAN_FilterInitStructure);

    filterCnt++;
}
