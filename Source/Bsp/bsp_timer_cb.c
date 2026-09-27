#include "bsp_timer_cb.h"

static volatile uint64_t timer_us_counter;

/**
  ******************************************************************************
  * @brief  timer8 中断回调
  * @param  None.
  * @retval None.
  ******************************************************************************/
void bsp_timer8_irq_cb(void)
{
	if (TIM_GetIntStatus(TIM8, TIM_INT_UPDATE) != RESET)
	{
		TIM_ClrIntPendingBit(TIM8, TIM_INT_UPDATE);		//清除标志位
		timer_us_counter++;
	#if 0
		if(timer_us_counter == 1)
		{
			ADC_TEST_IO_HIGH();
		}
		else
		{
			ADC_TEST_IO_LOW();
			timer_us_counter = 0;
		}
	#endif			
	}
}

/**
  ******************************************************************************
  * @brief  获取timer 10us级计数
  * @param  None.
  * @retval None.
  ******************************************************************************/
uint64_t timer_10us_get(void)
{
	return timer_us_counter;
}