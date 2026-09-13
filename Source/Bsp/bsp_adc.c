#include "bsp_adc.h"

adc_irq_cb_t adc_irq_cb = {NULL, NULL};

volatile uint16_t ADC_RegularConvertedValueTab[3];			//规则通道 DMA搬运使用 母线电压 温度 调速电位器
volatile uint16_t ADC_InjectedConvertedValueTab[4];			//注入通道 

/**
  ******************************************************************************
  * @brief  adc时钟配置
  * @param  None.
  * @retval None.
  ******************************************************************************/
static void bsp_adc_rcc_config(void)
{
		// Enable GPIOC clocks 
    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_AFIO |RCC_APB2_PERIPH_GPIOA | RCC_APB2_PERIPH_GPIOC, ENABLE);
		// Enable DMA clocks 
		RCC_EnableAHBPeriphClk(RCC_AHB_PERIPH_DMA, ENABLE);
	
		// Enable ADC clocks 
    RCC_EnableAHBPeriphClk(RCC_AHB_PERIPH_ADC ,ENABLE);   				//工作时钟源     108MHZ

    // RCC_ADCHCLK_DIV2
    RCC_ConfigAdcHclk(RCC_ADCHCLK_DIV2);				  								//采样时钟源     54MHZ
    RCC_ConfigAdc1mClk(RCC_ADC1MCLK_SRC_HSI, RCC_ADC1MCLK_DIV16);	//1M计数时钟源
}

/**
  ******************************************************************************
  * @brief  adc io配置
  * @param  None.
  * @retval None.
  ******************************************************************************/
void bsp_adc_io_config(void)
{
	GPIO_InitType GPIO_InitStructure = {0};
	
    GPIO_InitStruct(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Current = GPIO_DC_4mA;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Analog;
        
    GPIO_InitStructure.Pin       = GPIO_PIN_0;			//BEMF_U		ADC_IN1		PA0
    GPIO_InitPeripheral(GPIOA, &GPIO_InitStructure);
	
    GPIO_InitStructure.Pin       = GPIO_PIN_1;			//BEMF_V		ADC_IN2		PA1
    GPIO_InitPeripheral(GPIOA, &GPIO_InitStructure);
	
    GPIO_InitStructure.Pin       = GPIO_PIN_5;			//BEMF_W		ADC_IN6		PA5
    GPIO_InitPeripheral(GPIOA, &GPIO_InitStructure);
	
		GPIO_InitStructure.Pin       = GPIO_PIN_6;			//V_BUS			ADC_IN7		PA6
    GPIO_InitPeripheral(GPIOA, &GPIO_InitStructure);
	
		GPIO_InitStructure.Pin       = GPIO_PIN_0;			//Temperature	ADC_IN11	PC0
    GPIO_InitPeripheral(GPIOC, &GPIO_InitStructure);
	
		GPIO_InitStructure.Pin       = GPIO_PIN_4;			//Speed			ADC_IN15	PC4
    GPIO_InitPeripheral(GPIOC, &GPIO_InitStructure);
}

/**
  ******************************************************************************
  * @brief  adc dma 配置
  * @param  None.
  * @retval None.
  ******************************************************************************/
void bsp_adc_dma_config(void)
{
    DMA_InitType DMA_InitStructure;
    DMA_DeInit(DMA_CH1);
    DMA_InitStructure.PeriphAddr     = (uint32_t)&ADC->DAT;											//DMA外设地址
    DMA_InitStructure.MemAddr        = (uint32_t)ADC_RegularConvertedValueTab;  //DMA内存地址
    DMA_InitStructure.Direction      = DMA_DIR_PERIPH_SRC;											//DMA传输方向为外设到内存
    DMA_InitStructure.BufSize        = 3;																				//目的缓冲区元素数，也就是内存缓冲区元素数
    DMA_InitStructure.PeriphInc      = DMA_PERIPH_INC_DISABLE;									//外设地址递增模式禁止
    DMA_InitStructure.DMA_MemoryInc  = DMA_MEM_INC_ENABLE;											//内存地址递增模式开启
    DMA_InitStructure.PeriphDataSize = DMA_PERIPH_DATA_SIZE_HALFWORD;  					//外设数据长度为半字：2字节
    DMA_InitStructure.MemDataSize    = DMA_MemoryDataSize_HalfWord;							//内存数据长度为半字：2字节
    DMA_InitStructure.CircularMode   = DMA_MODE_CIRCULAR;												//DMA模式为循环模式
    DMA_InitStructure.Priority       = DMA_PRIORITY_HIGH;												//DMA优先级设置为高
    DMA_InitStructure.Mem2Mem        = DMA_M2M_DISABLE;													//内存到内存传输方式禁止
    DMA_Init(DMA_CH1, &DMA_InitStructure);
	
		/*DMA重映射请求：将DMA CH1通道映射到ADC1*/
		DMA_RequestRemap(DMA_REMAP_ADC1, DMA, DMA_CH1, ENABLE);
		/* Enable DMA channel1 */
		DMA_EnableChannel(DMA_CH1, ENABLE);
}

/**
  ******************************************************************************
  * @brief  adc配置
  * @param  None.
  * @retval None.
  ******************************************************************************/
static void bsp_adc_config(void)
{
    ADC_InitType ADC_InitStructure;
    NVIC_InitType NVIC_InitStructure;
    
    /* ADC regular sequencer */
    ADC_InitStructure.MultiChEn      = ENABLE;    						//指定转换是单通道和多通道选择，enable为选择多通道
    ADC_InitStructure.ContinueConvEn = DISABLE;	  						//指定转换是是单次转换还是连续转换，disable为单次转换
    ADC_InitStructure.ExtTrigSelect  = ADC_EXT_TRIGCONV_NONE; //规则通道外部触发源选择：不使用外部触发源触发规则通道的转换
    ADC_InitStructure.DatAlign       = ADC_DAT_ALIGN_R;		    //数据为右对齐的方式
    ADC_InitStructure.ChsNumber      = 3;					            //规则通道的通道数为3个
    ADC_Init(ADC, &ADC_InitStructure);
    /* ADC regular channel configuration */
	/*
		ADC_CH_7_PA6：ADC通道
		ADC_SAMP_TIME_13CYCLES5:采样时间值
	*/
    ADC_ConfigRegularChannel(ADC, ADC_CH_7_PA6 , 1, ADC_SAMP_TIME_13CYCLES5);//V_BUS
    ADC_ConfigRegularChannel(ADC, ADC_CH_11_PC0, 2, ADC_SAMP_TIME_13CYCLES5);//Temperature
    ADC_ConfigRegularChannel(ADC, ADC_CH_15_PC4, 3, ADC_SAMP_TIME_13CYCLES5);//Speed

    /* Set injected sequencer length：为注入通道配置序列器长度 */
    ADC_ConfigInjectedSequencerLength(ADC, 4);
    /* ADC injected channel Configuration */
    ADC_ConfigInjectedChannel(ADC, ADC_CH_3_PA2, 1, ADC_SAMP_TIME_13CYCLES5);//Current->OPA->OPAOUT:PA2
    ADC_ConfigInjectedChannel(ADC, ADC_CH_1_PA0, 2, ADC_SAMP_TIME_13CYCLES5);//BEMF_U
    ADC_ConfigInjectedChannel(ADC, ADC_CH_2_PA1, 3, ADC_SAMP_TIME_13CYCLES5);//BEMF_V
	ADC_ConfigInjectedChannel(ADC, ADC_CH_6_PA5, 4, ADC_SAMP_TIME_13CYCLES5);//BEMF_W
    /* ADC injected external trigger configuration */
    ADC_ConfigExternalTrigInjectedConv(ADC, ADC_EXT_TRIG_INJ_CONV_T1_CC4);  //配置为Timer1的通道4触发
    /* Enable automatic injected conversion start after regular one 
		自动注入：通过启动规则通道转换后，注入通道开始自动启动转换
		触发注入：通过触发源也就是T1_CC4触发注入通道转换
	*/
    ADC_EnableAutoInjectedConv(ADC, DISABLE);


    /* Enable ADC */
    ADC_Enable(ADC, ENABLE);
    /* Check ADC Ready */
    while(ADC_GetFlagStatusNew(ADC,ADC_FLAG_RDY) == RESET);
	
    /* Start ADC calibration */
    ADC_StartCalibration(ADC);
    /* Check the end of ADC calibration */
    while (ADC_GetCalibrationStatus(ADC));

    /* Configure and enable ADC interrupt */
    NVIC_InitStructure.NVIC_IRQChannel                   = ADC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    /* Enable JEOC interrupt */
    ADC_ConfigInt(ADC, ADC_INT_JENDC, ENABLE);
    
    /* Enable ADC DMA */
    ADC_EnableDMA(ADC, ENABLE);
    /* Enable ADC external trigger */
    ADC_EnableExternalTrigConv(ADC, ENABLE);
	/* Enable ADC inj external trigger */
	ADC_EnableExternalTrigInjectedConv(ADC,ENABLE);    
}


/**
  ******************************************************************************
  * @brief  adc初始化
  * @param  irq_cb:中断回调指针
  * @param  formal_param:中断回调指针数组中形参
  * @retval None.
  ******************************************************************************/
void bsp_adc_init(void (*irq_cb)(void (*formal_param)(void)), void (*formal_param)(void))
{	
	if((irq_cb == NULL) || (formal_param == NULL))
	{
		while(1);
	}
	adc_irq_cb.adc_cb = irq_cb;				//回调赋值
	adc_irq_cb.formal_param = formal_param;	
	bsp_adc_rcc_config();
	bsp_adc_io_config();
	bsp_adc_config();
	bsp_adc_dma_config();
}