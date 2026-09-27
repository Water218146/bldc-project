#include "bsp_pwm.h"

#include "bsp_pwm.h"
#include <stdio.h>

#define MAIN_FREQUENCY	    (108000000) 	    //主频 Ft 108M
#define PWM_FREQUENCY 		20000 		    		//载频  Fpwm 控制mos管的频率 20K

pwm_irq_cb_t pwm_irq_cb = {NULL, NULL};

static uint32_t TimerPeriod = 0; 				//定时器自动重装载的arr值

/**
  ******************************************************************************
  * @brief  pwm时钟配置 io timer1高级定时器
  * @param  None.
  * @retval None.
  ******************************************************************************/
static void bsp_pwm_rcc_config(void)
{
	// Enable GPIO clocks 
    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_AFIO | RCC_APB2_PERIPH_GPIOA | RCC_APB2_PERIPH_GPIOB, ENABLE);
	// Enable TIM1 clocks 
    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_TIM1, ENABLE);
}

/**
  ******************************************************************************
  * @brief  pwm配置 io timer1高级定时器
  * @param  None.
  * @retval None.
  ******************************************************************************/
static void bsp_pwm_io_config(void)
{
	GPIO_InitType GPIO_InitStructure = {0};

	// Configure TIM1, CH1(PA8),CH2(PA9),CH3(PA10),CH4(PA11) as alternate function push-pull
    GPIO_InitStruct(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Current = GPIO_DC_4mA;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Alternate = GPIO_AF2_TIM1;
    GPIO_InitStructure.Pin        = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11;
    GPIO_InitPeripheral(GPIOA, &GPIO_InitStructure);

#if 0 
	// Configure TIM1, CH1N(PB13),CH2N(PB14),CH3N(PB15) as alternate function push-pull
    GPIO_InitStructure.Pin        = GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
    GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure);
#else	//上管PWM控制,下关常开或常闭方式控制	
		GPIO_InitStruct(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Current = GPIO_DC_4mA;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.Pin        = GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
    GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure);
#endif	
	
#if 1	
	//PB12--tim1 break IO
    GPIO_InitStruct(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Current = GPIO_DC_4mA;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Alternate = GPIO_AF5_TIM1;
    GPIO_InitStructure.Pin       = GPIO_PIN_12;
		GPIO_InitStructure.GPIO_Pull = GPIO_Pull_Down;
    GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure);
#endif	
}

/**
  ******************************************************************************
  * @brief  pwm配置 timer1
  * @param  None.
  * @retval None.
  ******************************************************************************/
void bsp_pwm_config(void)
{
	TIM_TimeBaseInitType TIM1_TimeBaseStructure;	//定时器初始化结构体
	OCInitType TIM1_OCInitStructure;              //output compare：输出比较 决定占空比
	TIM_BDTRInitType TIM1_BDTRInitStructure;			//高级定时器刹车配置结构体
	NVIC_InitType NVIC_InitStructure;							//中断配置结构体
	
	//自动重装载值 计算的出TimerPeriod = 2699
	TimerPeriod = (MAIN_FREQUENCY / (PWM_FREQUENCY * 2)) - 1;		//PWM_FREQUENCY为20Khz 中心对齐模式下 频率会减半 所以要想实际要想生成20Khz的PWM 需要(PWM_FREQUENCY * 2)
	my_printf(DEBUG_COM,"TimerPeriod:%d , Fpwm = %d\r\n",TimerPeriod,PWM_FREQUENCY);		//打印Pwm参数
	
	//Time Base configuration
	TIM_DeInit(TIM1);
	TIM_InitTimBaseStruct(&TIM1_TimeBaseStructure);
	TIM1_TimeBaseStructure.Prescaler = 0;													//预分频值：对定时器时钟做分频 此处不分频  108MHZ
	TIM1_TimeBaseStructure.CntMode = TIM_CNT_MODE_CENTER_ALIGN1;	//计数器计数模式：中心对齐模式1
//	TIM1_TimeBaseStructure.CntMode = TIM_CNT_MODE_UP;						//向上计数模式
	TIM1_TimeBaseStructure.Period = TimerPeriod;									//周期值：这个值写入到自动重装载寄存器 arr
	TIM1_TimeBaseStructure.ClkDiv = TIM_CLK_DIV1;									//时钟分频：这里1分频也就是不做分频
	TIM1_TimeBaseStructure.RepetCnt = 0;													//重复计数器：将重复计数器值设置为0	
		
	TIM_InitTimeBase(TIM1, &TIM1_TimeBaseStructure);
	//Channel 1, 2,3 in PWM mode
	TIM_InitOcStruct(&TIM1_OCInitStructure);	
	TIM1_OCInitStructure.OcMode = TIM_OCMODE_PWM1;									//Pos logic(when '<' is active(激活活动),when '>' is inactive(不活跃的，未激活的))
	TIM1_OCInitStructure.OutputState = TIM_OUTPUT_STATE_ENABLE; 		//CH1-CH3输出通道开启
	TIM1_OCInitStructure.OutputNState = TIM_OUTPUT_NSTATE_DISABLE;	//三相下桥输出关闭               
	TIM1_OCInitStructure.Pulse = 0;																	//初始U、V、W三路PWM通道输出占空比为0
	TIM1_OCInitStructure.OcPolarity = TIM_OC_POLARITY_HIGH;					//OC有效电平为高电平 即arr的计数值小于pulse时 引脚输出高电平
	TIM1_OCInitStructure.OcNPolarity = TIM_OCN_POLARITY_HIGH; 			//
	TIM1_OCInitStructure.OcIdleState = TIM_OC_IDLE_STATE_RESET;			//初始状态或占空比值为0时CH1-CH4通道输出低电平
	TIM1_OCInitStructure.OcNIdleState = TIM_OC_IDLE_STATE_RESET;    //初始状态或占空比值为0时CH1N-CH3N通道输出低电平        
	TIM_InitOc1(TIM1, &TIM1_OCInitStructure); 											//将配置应用到 Oc1 即TIM1的CH1
	TIM_InitOc2(TIM1, &TIM1_OCInitStructure);												//将配置应用到 Oc2 即TIM1的CH2
	TIM_InitOc3(TIM1, &TIM1_OCInitStructure);												//将配置应用到 Oc3 即TIM1的CH3
	
	//Channel 4 Configuration in OC 
	TIM1_OCInitStructure.OcMode = TIM_OCMODE_PWM1;									//Pos logic(when '<' is active(激活活动),when '>' is inactive(不活跃的，未激活的))
	TIM1_OCInitStructure.OutputState = TIM_OUTPUT_STATE_ENABLE;			//CH4输出通道开启
	TIM1_OCInitStructure.Pulse = TimerPeriod >> 3;// 2699 / 8;
	TIM_InitOc4(TIM1, &TIM1_OCInitStructure);
	
	//	//Enables the TIM1 Preload on CC1,CC2,CC3,CC4 Register
//	TIM_ConfigOc1Preload(TIM1, TIM_OC_PRE_LOAD_ENABLE);
//	TIM_ConfigOc2Preload(TIM1, TIM_OC_PRE_LOAD_ENABLE);
//	TIM_ConfigOc3Preload(TIM1, TIM_OC_PRE_LOAD_ENABLE);
#if 1
	//Automatic Output enable, Break, dead time and lock configuration
	TIM1_BDTRInitStructure.OssrState = TIM_OSSR_STATE_ENABLE;      		//默认设置为enable
	TIM1_BDTRInitStructure.OssiState = TIM_OSSI_STATE_ENABLE;					//默认设置为enable
	TIM1_BDTRInitStructure.LockLevel = TIM_LOCK_LEVEL_OFF; 						//防止软件错误写保护，设置为关闭
	TIM1_BDTRInitStructure.DeadTime = 0;															//死区时间：未用到					
	TIM1_BDTRInitStructure.Break = TIM_BREAK_IN_ENABLE;               //刹车输入使能
	TIM1_BDTRInitStructure.BreakPolarity = TIM_BREAK_POLARITY_HIGH;   //刹车信号为高电平有效	
	/*
	TIM_AUTO_OUTPUT_ENABLE: 当刹车信号触发后(PB12 0->1)，6路PWM输出全部关闭输出。
						    当刹车信号解除后(PB12 1->0)，6路PWM正常输出
	TIM_AUTO_OUTPUT_DISABLE:当刹车信号触发后(PB12 0->1)，6路PWM输出全部关闭输出。
							当刹车信号解除后(PB12 1->0)，6路PWM保持触发后状态，全部关闭输出
								此时需要注意的是：PWM输出关闭后，TIM1_CH4通道也无法输出PWM，
								所以ADC无法通过TIM1_CH4上升沿通道触发进行采集转换
	*/
	TIM1_BDTRInitStructure.AutomaticOutput = TIM_AUTO_OUTPUT_ENABLE;
	/*false:使用内部的比较器输出作为刹车信号 true:使用外部IO输入作为刹车信号*/
    TIM1_BDTRInitStructure.IomBreakEn = true;								
    TIM1_BDTRInitStructure.LockUpBreakEn = false;										//LockUp刹车关闭
    TIM1_BDTRInitStructure.PvdBreakEn = false;											//PVD刹车关闭
		TIM_ConfigBkdt(TIM1, &TIM1_BDTRInitStructure);
	
	/*IT about*/
    TIM_ClrIntPendingBit(TIM1, TIM_INT_BREAK);   										//清除brake刹车中断状态位
    TIM_ConfigInt(TIM1, TIM_INT_BREAK, ENABLE);											//开启brake刹车中断
	
	/*Enable the TIM1 BRK Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel                   = TIM1_BRK_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
#endif
	//TIM1 counter enable
	TIM_Enable(TIM1, ENABLE);
	TIM_EnableCtrlPwmOutputs(TIM1,ENABLE);
#if 0	
	bsp_pwm_duty_set((TimerPeriod + 1) >> 4);
#else
	bsp_pwm_duty_set(0);
#endif
}	

/**
  ******************************************************************************
  * @brief  pwm初始化 timer1
  * @param  irq_bk_cb:timer brake中断回调指针
  * @param  irq_cb:timer中断回调指针
  * @retval None.
  ******************************************************************************/
void bsp_pwm_init(void (*irq_bk_cb)(void), void (*irq_cb)(void))
{
	if((irq_bk_cb == NULL) || (irq_cb == NULL))
	{
		while(1);
	}
	pwm_irq_cb.pwm_bk_cb = irq_bk_cb; 
	pwm_irq_cb.pwm_cb    = irq_cb;
	bsp_pwm_rcc_config();
	bsp_pwm_io_config();
	bsp_pwm_config();
}

/**
  ******************************************************************************
  * @brief  pwm占空比设置 timer1
  * @param  duty：设置pwm占空比值
  * @retval None.
  ******************************************************************************/
void bsp_pwm_duty_set(uint16_t duty)
{
	/*pwm幅值限幅判断*/
	if(duty > PWM_PERIOD_MAX)
	{
		duty = PWM_PERIOD_MAX;
	}
	TIM1->CCDAT1 = duty; // U相占空比
	TIM1->CCDAT2 = duty; // V相占空比
	TIM1->CCDAT3 = duty; // W相占空比
	
	
	if(duty == 0)
	{
		/*
		占空比为0条件下：需要ch4上升沿触发adc转换，此时ch4占空比不能为0,
		默认占空比设置为：满占空比/8 = 12.5%
		*/
		duty = PWM_PERIOD_MAX;
	}
	/*设定timer1 ch4通道占空比，此通道pwm上升沿触发ADC采集*/
	TIM1->CCDAT4 = duty >> 3;//timer1 ch4上升沿触发adc转换pwm占空比  中心对齐模式下 使CH4触发ADC采集的时刻靠近每一相PWM导通的中心 保证采集到的是MOS管完全导通且稳定的电压
}

/**
  ******************************************************************************
  * @brief  3路pwm输出全部关闭 timer1
  * @param  None.
  * @retval None.
  ******************************************************************************/
void bsp_all_pwm_close(void)
{
    uint16_t tmp;

	tmp = TIM1->CCEN;
	tmp &= (uint16_t)(~((uint16_t)TIM_CCEN_CC1EN));
	tmp &= (uint16_t)(~((uint16_t)TIM_CCEN_CC2EN));
	tmp &= (uint16_t)(~((uint16_t)TIM_CCEN_CC3EN));
	TIM1->CCEN = tmp;
}

/**
  ******************************************************************************
  * @brief  3路pwm输出全部开启 timer1
  * @param  None.
  * @retval None.
  ******************************************************************************/
void bsp_all_pwm_open(void)
{
  uint16_t tmp;

	tmp = TIM1->CCEN;
	tmp |= (uint16_t)(((uint16_t)TIM_CCEN_CC1EN));
	tmp |= (uint16_t)(((uint16_t)TIM_CCEN_CC2EN)); 
	tmp |= (uint16_t)(((uint16_t)TIM_CCEN_CC3EN)); 
	TIM1->CCEN = tmp;
}

/**
  ******************************************************************************
  * @brief  pwm输出频率设置 timer1
  * @param  None.
  * @retval None.
  ******************************************************************************/
void bsp_set_pwm_freq(uint16_t freq)
{
	uint16_t freq_temp = 0;
	
	freq_temp = ((uint16_t)MAIN_FREQUENCY >> 1) / freq;
	
	TIM1->AR = freq_temp;
	TIM1->CCDAT4 = (TIM1->AR - 30);
}