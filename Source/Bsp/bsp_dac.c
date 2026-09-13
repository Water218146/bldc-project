#include "bsp_dac.h"

/**
  ******************************************************************************
  * @brief  dac时钟配置
  * @param  None.
  * @retval None.
  ******************************************************************************/
static void bsp_dac_rcc_config(void)
{
	/* DAC Periph clock enable */
    RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_DAC, ENABLE);
	
	/* GPIOA Periph clock enable */
    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOA, ENABLE);
}	
  
/**
  ******************************************************************************
  * @brief  dac io配置
  * @param  None.
  * @retval None.
  ******************************************************************************/
static void bsp_dac_io_config(void)
{
	GPIO_InitType GPIO_InitStructure = {0};

    GPIO_InitStruct(&GPIO_InitStructure);
	
    /* Once the DAC channel is enabled, the corresponding GPIO pin is automatically
       connected to the DAC converter. In order to avoid parasitic consumption,
       the GPIO pin should be configured in analog */
    GPIO_InitStructure.Pin       = GPIO_PIN_4 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Analog;
    GPIO_InitStructure.GPIO_Pull = GPIO_No_Pull;
    GPIO_InitPeripheral(GPIOA, &GPIO_InitStructure);
}	

/**
  ******************************************************************************
  * @brief  dac 配置
  * @param  dac_val:dac输出值
  * @retval None.
  ******************************************************************************/
static void bsp_dac_config(uint16_t dac_val)
{
	DAC_InitType DAC_InitStructure;
	
    /* DAC channel1 Configuration */
    DAC_InitStructure.Trigger          = DAC_TRG_SOFTWARE;     	//DAC通过软件方式触发
    DAC_InitStructure.WaveGen          = DAC_WAVEGEN_NONE;     	//禁用噪声和三角波
    DAC_InitStructure.LfsrUnMaskTriAmp = DAC_UNMASK_LFSRBIT0;  	//WaveGen设置为DAC_WAVEGEN_NONE后设置LfsrUnMaskTriAmp任意值始终为无效状态
    DAC_InitStructure.BufferOutput     = DAC_BUFFOUTPUT_DISABLE;//禁用 DAC 通道输出缓存
    DAC_Init(&DAC_InitStructure);
	
    /* Enable DAC Channel1: Once the DAC channel1 is enabled, PA.04 is
             automatically connected to the DAC converter. */
		DAC_Enable(ENABLE);
		bsp_delay_ms(10);   												  //等待DAC完全打开 tWAKEUP时间
    /* Set DAC  channel DR12DCH register */
    DAC_SetChData(DAC_ALIGN_R_12BIT, dac_val);    //1.0V   241mv
		DAC_SoftTrgEnable(ENABLE);  									//此步骤不能省略，开启软件触发DAC	
}

/**
  ******************************************************************************
  * @brief  dac 初始化
  * @param  dac_val:dac输出值
  * @retval None.
  ******************************************************************************/
void bsp_dac_init(uint16_t dac_val)
{
	bsp_dac_rcc_config();
	bsp_dac_io_config();
	bsp_dac_config(dac_val);
}




