#include "bsp_pwm_cb.h"

/**
  ******************************************************************************
  * @brief  pwm brake中断回调 breke引脚检测到上升沿时触发
  * @param  None.
  * @retval None.
  ******************************************************************************/
void bsp_pwm_brake_irq_cb(void)
{
  /* 检测到母线电流过流时会触发该中断 */
	if(TIM_GetIntStatus(TIM1, TIM_INT_BREAK) != RESET)
	{
		TIM_ClrIntPendingBit(TIM1, TIM_INT_BREAK);
		motor_stop();	
	}
}

/**
  ******************************************************************************
  * @brief  pwm 中断回调
  * @param  None.
  * @retval None.
  ******************************************************************************/
void bsp_pwm_irq_cb(void)
{
	
}
