#include "bsp_io.h"


/**
  ******************************************************************************
  * @brief  io初始化
  * @param  None.
  * @retval None.
  ******************************************************************************/
	
void bsp_io_init(void)
{
	GPIO_InitType GPIO_InitStructure;
	//Enable GPIO Clocks
	RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOA|RCC_APB2_PERIPH_GPIOB|RCC_APB2_PERIPH_GPIOC|RCC_APB2_PERIPH_GPIOD,ENABLE);
	//IO Config
	GPIO_InitStruct(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Current = GPIO_DC_4mA;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pull = GPIO_No_Pull;
	GPIO_InitStructure.Pin = GPIO_PIN_5;
	GPIO_InitPeripheral(GPIOC,&GPIO_InitStructure);
#if 0
	GPIO_InitStruct(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Current = GPIO_DC_4mA;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Input;
    GPIO_InitStructure.Pin       = GPIO_PIN_12;
    GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure);
#endif		
}