#include "dma.h"

void DMA_ADC1_Config(){
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);


}
