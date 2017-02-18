#include "stm32f10x.h"
#include "STM32F103C8T6_Function.h"

static uint32_t TimingDelay = 0;
void delay(uint32_t nTime);

int main(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2ENR_IOPBEN,ENABLE);
	//pinSetup(GPIOB,GPIO_Pin_0,GPIO_Mode_Out_PP);

	SysTick_Config(SystemCoreClock/1000);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);



    while(1)
    {
    }
}
