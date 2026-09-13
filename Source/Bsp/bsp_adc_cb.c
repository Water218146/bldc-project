#include "bsp_adc_cb.h"

adc_digital_val_e adc_digital_val;

/**
  ******************************************************************************
  * @brief  adc中断回调
  * @param  bldc_sensorless_algorithm_func_cb:无霍尔传感器算法执行回调
  * @retval None.
  ******************************************************************************/
void bsp_adc_irq_cb(void (*bldc_sensorless_algorithm_func_cb)(void))
{
	if(ADC_GetIntStatus(ADC, ADC_INT_JENDC) == SET)							//查看转换完成标志位
	{
//		ADC_TEST_IO_HIGH();
		ADC_ClearFlag(ADC, ADC_FLAG_JENDC);												//清除转换完成标志位

		/* 直接通过库函数获取注入通道的数据 */
		ADC_InjectedConvertedValueTab[0] = ADC_GetInjectedConversionDat(ADC, ADC_INJ_CH_1);//Current->OPA->OPAOUT:PA2
		ADC_InjectedConvertedValueTab[1] = ADC_GetInjectedConversionDat(ADC, ADC_INJ_CH_2);//BEMF_U
		ADC_InjectedConvertedValueTab[2] = ADC_GetInjectedConversionDat(ADC, ADC_INJ_CH_3);//BEMF_V
		ADC_InjectedConvertedValueTab[3] = ADC_GetInjectedConversionDat(ADC, ADC_INJ_CH_4);//BEMF_W
		
		/* 将DMA中存储的数据拿过来用并存储*/
		adc_digital_val.v_bus       = ADC_RegularConvertedValueTab[0];//V_BUS
		adc_digital_val.temperature = ADC_RegularConvertedValueTab[1];//T
		adc_digital_val.speed       = ADC_RegularConvertedValueTab[2];//SPEED
		
		/* 存储注入通道的数据 */
		adc_digital_val.current = ADC_InjectedConvertedValueTab[0];	//Current -> OPA -> OPAOUT:PA2
		adc_digital_val.bemf_u  = ADC_InjectedConvertedValueTab[1];	//BEMF_U
		adc_digital_val.bemf_v  = ADC_InjectedConvertedValueTab[2];	//BEMF_V
		adc_digital_val.bemf_w  = ADC_InjectedConvertedValueTab[3];	//BEMF_W
		
		bldc_sensorless_algorithm_func_cb();				//执行无霍尔传感器算法
		
    ADC_EnableSoftwareStartConv(ADC,ENABLE);    //通过软件触发的方式启动规则通道ADC转换
//		ADC_TEST_IO_LOW();
//		ttt_cnt++;
	}
	else 
	{
		if(ADC_GetIntStatus(ADC, ADC_INT_AWD) == SET)
		{
            ADC_ClearFlag(ADC, ADC_FLAG_AWDG);
		}
	}
}