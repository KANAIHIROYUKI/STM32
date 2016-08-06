#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

int main(void)
{
    HAL_Init();

    BSP_LED_Init(LED3);

    __RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pin  = GPIO_PIN_0;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA,GPIO_InitStructure);

    for(;;){
      HAL_Delay(500);
      BSP_LED_Toggle(LED3);
    }
}
