#include "CAN.h"

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
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
	CAN_InitStructure.CAN_BS1 = CAN_BS1_3tq;
	CAN_InitStructure.CAN_BS2 = CAN_BS2_5tq;
	CAN_InitStructure.CAN_Prescaler = 2;
	CAN_Init(CAN1,&CAN_InitStructure);

	/*
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
    CAN_FilterInitStructure.CAN_FilterNumber = 0;
    CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList;
    CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_16bit;
    CAN_FilterInitStructure.CAN_FilterIdLow = (0x100+0)<<5;
    CAN_FilterInitStructure.CAN_FilterMaskIdLow = (0x100+1)<<5;
    CAN_FilterInitStructure.CAN_FilterIdHigh = (0x100+2)<<5;
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh = (0x100+3)<<5;
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
    CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
    CAN_FilterInit(&CAN_FilterInitStructure);*/


	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
    CAN_FilterInitStructure.CAN_FilterNumber = 0;
    CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
    CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
    CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
    CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
    CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
    CAN_FilterInit(&CAN_FilterInitStructure);


    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);
}

void CAN1Send(uint16_t id,uint8_t length,uint8_t data[8]){
    CanTxMsg TxMessage;
    TxMessage.StdId = id;
    TxMessage.IDE   = CAN_ID_STD;
    if(length == 0){
    	TxMessage.RTR = CAN_RTR_REMOTE;
    }else{
    	TxMessage.RTR   = CAN_RTR_DATA;
    }
    TxMessage.DLC   = length;
    TxMessage.Data[0] = data[0];
    TxMessage.Data[1] = data[1];
    TxMessage.Data[2] = data[2];
    TxMessage.Data[3] = data[3];
    TxMessage.Data[4] = data[4];
    TxMessage.Data[5] = data[5];
    TxMessage.Data[6] = data[6];
    TxMessage.Data[7] = data[7];

    CAN_Transmit(CAN1,&TxMessage);
}
