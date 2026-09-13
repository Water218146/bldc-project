#include "bsp_systick.h"


volatile uint32_t n_tick = 0;

/**
 * @brief  初始化系统滴答定时器
 * @param  无
 * @retval 无
 */
void bsp_systick_init(void)
{
    if(SysTick_Config(SystemCoreClock / 1000) != 0) //设置系统滴答定时器中断周期为1ms
    {
        // 初始化失败
        while(1);
    }
}

/**
 * @brief  系统滴答定时器关闭
 * @param  无
 * @retval 无
 */
void bsp_systick_disable(void)
{
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; // 禁用SysTick定时器
}

/**
 * @brief  获取系统滴答定时器计数值
 * @param  无
 * @retval 系统滴答定时器计数值
 */
uint32_t bsp_systick_get_tick(void)
{
    return n_tick;
}

/**
 * @brief  延时函数，阻塞式，单位为毫秒
 * @param  ms: 延时时间，单位为毫秒
 * @retval 无
 */
void bsp_delay_ms(uint32_t ms)
{
    uint32_t tick = bsp_systick_get_tick();
    while((bsp_systick_get_tick() - tick) < ms);
}
