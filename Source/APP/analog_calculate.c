#include "analog_calculate.h"

//声明一个实际应用的数组
adc_voltage_val_e adc_voltage_val;

#define R23 33.0f		
#define R30 3.0f

/**
  ******************************************************************************
  * @brief  adc值计算函数
  * @param  None.
  * @retval None.
  ******************************************************************************/
void adc_value_calculate(void)
{
	uint16_t adc_val = 0;
	static uint16_t adc_v_bus = 0;
	/* 计算母线电压 */
	{
		#if 0		//母线电压不使用滤波算法
			adc_val = adc_digital_val.v_bus;//获取v_bus的数字原始值
			adc_voltage_val.v_bus = (float)adc_val / 4095.0f * 3.3f / (R30 / (R23 + R30));//通过分压节点电压反推出母线电压	
		#else
			adc_v_bus = LPF_Calc(adc_digital_val.v_bus,adc_v_bus);
			adc_voltage_val.v_bus = (float)adc_v_bus / 4095.0f * 3.3f / (R30 / (R23 + R30));//通过分压节点电压反推出母线电压	
		#endif
	}

	/* NTC热敏电阻温度计算 */
	{
		//NTC温度计算  RT＝R0*exp(B (1/T-1/T0)) RT为NTC当前环境温度下的电阻值 B为温度系数 T当前环境温度  T0手册中能查到的温度（K） R0环境温度为T0时的电阻值 
		float Rt = 0.0f;						//NTC电阻
		float R0 = 10000.0f;				//T0 温度下NTC的阻值
		float T0 = 273.15 + 25.0; 	//T0 温度转化为开尔文温度
		float B = 3455.0f;					//B值
		float Ka = 273.15;					//K值
		float VR = 0.0f;						//NTC电压值
		VR = (float)adc_digital_val.temperature / 4095.0f * 3.3f ;
		Rt = 10000 * VR / (3.3 - VR);		//计算当前温度下的NTC的温度值
		adc_voltage_val.temperature = (1.0f / ((log(Rt / R0) / B) + (1.0f / T0)))-Ka;	
	}

	/* 母线电流计算 */
	{
		adc_val = adc_digital_val.current;
		adc_voltage_val.current = ((float)adc_val / 4095.0f) * 3.3 / 32.0f / 0.002;
	}

	
}
	
/**
  ******************************************************************************
  * @brief  adc值计算任务 50ms一次
  * @param  None.
  * @retval None.
  ******************************************************************************/	
void adc_calculate_task(void)
{
	static uint32_t timeout = 0;
	adc_value_calculate();
	
	#if 1
	if(n_tick - timeout >= 500)
	{
		timeout = n_tick;
		// my_printf(DEBUG_COM,"V_BUS:%.2f,V_Bus_raw:%d\r\n",adc_voltage_val.v_bus,adc_digital_val.v_bus);
		// my_printf(DEBUG_COM,"temperature:%.2f,temperature_raw:%d\r\n",adc_voltage_val.temperature,adc_digital_val.temperature);	
	}
	#endif
}