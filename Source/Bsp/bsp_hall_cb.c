#include "bsp_hall_cb.h"


/* 定义指向U、V、W霍尔传感器中断回调函数的指针数组 */
void (*hall_uvw_irq_cb[3])(void (*bldc_sensor_algorithm_func_cb)(void)) = {
	bsp_hall_u_irq_cb,
	bsp_hall_v_irq_cb,
	bsp_hall_w_irq_cb,
};



/**
  ******************************************************************************
  * @brief  hall u相中断回调
  * @param  bldc_sensor_algorithm_func_cb:带霍尔传感器算法执行回调
  * @retval None.
  ******************************************************************************/
void bsp_hall_u_irq_cb(void (*bldc_sensor_algorithm_func_cb)(void))
{
	if(RESET != EXTI_GetITStatus(EXTI_LINE1))		//PC1
	{
		EXTI_ClrITPendBit(EXTI_LINE1);
		bldc_sensor_algorithm_func_cb();
	}
}

/**
  ******************************************************************************
  * @brief  hall v相中断回调
  * @param  bldc_sensor_algorithm_func_cb:带霍尔传感器算法执行回调
  * @retval None.
  ******************************************************************************/
void bsp_hall_v_irq_cb(void (*bldc_sensor_algorithm_func_cb)(void))
{
	if(RESET != EXTI_GetITStatus(EXTI_LINE2))   //PC2
    {
        EXTI_ClrITPendBit(EXTI_LINE2);
				bldc_sensor_algorithm_func_cb();
    }
}

/**
  ******************************************************************************
  * @brief  hall w相中断回调
  * @param  bldc_sensor_algorithm_func_cb:带霍尔传感器算法执行回调
  * @retval None.
  ******************************************************************************/
void bsp_hall_w_irq_cb(void (*bldc_sensor_algorithm_func_cb)(void))
{
	if(RESET != EXTI_GetITStatus(EXTI_LINE3))   //PC3
    {
        EXTI_ClrITPendBit(EXTI_LINE3);
				bldc_sensor_algorithm_func_cb();
    }
}