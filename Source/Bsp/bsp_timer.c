#include "bsp_timer.h"

#define TIMER8_PERIPH_FREQUENCY	    (108000000) 	    //timer8外设时钟频率
#define TIMER8_OUT_FREQUENCY 		(100000)			//timer8输出频率 100KHZ  10us

timer_irq_cb_t timer8_irq_cb = {NULL};

static uint16_t Timer8Period = 0;

/**
  ******************************************************************************
  * @brief  timer时钟配置
  * @param  None.
  * @retval None.
  ******************************************************************************/
static void bsp_timer8_rcc_config(void)
{
	// Enable TIM8 clocks 
    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_TIM8, ENABLE);
}

/**
  ******************************************************************************
  * @brief  timer8配置
  * @param  None.
  * @retval None.
  ******************************************************************************/
void bsp_timer8_config(void)
{
	TIM_TimeBaseInitType TIM8_TimeBaseStructure;
	NVIC_InitType NVIC_InitStructure;
	
	Timer8Period = TIMER8_PERIPH_FREQUENCY / TIMER8_OUT_FREQUENCY - 1;  

	TIM_DeInit(TIM8);
	TIM_InitTimBaseStruct(&TIM8_TimeBaseStructure);
	TIM8_TimeBaseStructure.Prescaler = 0;								//预分频值，对定时器时钟做分频 108MZ 此处不分频
	TIM8_TimeBaseStructure.CntMode   = TIM_CNT_MODE_UP;	//配置为向上计数模式
	TIM8_TimeBaseStructure.Period 	 = Timer8Period;		//周期值，这个值写入自动重装载寄存器
	TIM8_TimeBaseStructure.ClkDiv		 = TIM_CLK_DIV1;		//时钟分频，此处不分频
	TIM8_TimeBaseStructure.RepetCnt  = 0;								//重复计数器，重复计数器设置为0
	
	TIM_InitTimeBase(TIM8, &TIM8_TimeBaseStructure);
	
	TIM_ConfigInt(TIM8, TIM_INT_UPDATE, ENABLE);

	/*Enable the TIM1 BRK Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel                   = TIM8_UP_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 10;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	//TIM8 counter enable
	TIM_Enable(TIM8, ENABLE);	
}

/**
  ******************************************************************************
  * @brief  timer8初始化 
  * @param  irq_cb:timer中断回调指针
  * @retval None.
  ******************************************************************************/
void bsp_timer8_init(void (*irq_cb)(void))
{
	if(irq_cb == NULL)
	{
		while(1);
	}
	timer8_irq_cb.timer_cb = irq_cb;
	bsp_timer8_rcc_config();
	bsp_timer8_config();
}