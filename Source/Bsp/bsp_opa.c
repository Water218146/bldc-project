#include "bsp_opa.h"

/**
  ******************************************************************************
  * @brief  opa1时钟配置
  * @param  None.
  * @retval None.
  ******************************************************************************/
static void bsp_opa1_rcc_config(void)
{
	RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_OPAMP, ENABLE); //开启 APB1 上的 OPAMP 外设时钟
	
	RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_AFIO | RCC_APB2_PERIPH_GPIOA,ENABLE); //开启 AFIO 和 GPIOA 时钟，供 PA7 输入和 PA2 输出使用
}	
  
/**
  ******************************************************************************
  * @brief  opa1 io配置
  * @param  None.
  * @retval None.
  ******************************************************************************/
static void bsp_opa1_io_config(void)
{
	GPIO_InitType GPIO_InitStructure = {0};

	GPIO_InitStruct(&GPIO_InitStructure); 					 	 //使用驱动默认值初始化 GPIO 配置结构体
	OPAMP_SetVmSel(OPAMP1, OPAMPx_CS_VMSEL_FLOAT); 	 	 //PGA 使用内部反馈，外部负输入悬空，PA3 不参与运放输入
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Analog; 	 //运放输入引脚配置为模拟模式
	OPAMP_SetVpSel(OPAMP1, OPAMP1_CS_VPSEL_PA7); 				//选择 PA7 作为 OPAMP1 正输入 Vp
	GPIO_InitStructure.Pin = GPIO_PIN_7; 								//配置 OPAMP1 正输入引脚 PA7
  GPIO_InitPeripheral(GPIOA, &GPIO_InitStructure); 		//初始化 PA7
		
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Analog; 	//OPAMP1 输出 PA2 配置为模拟模式
	GPIO_InitStructure.GPIO_Current = GPIO_DC_4mA; 			//设置 PA2 的驱动能力为 4 mA
	GPIO_InitStructure.Pin        = GPIO_PIN_2; 				//OPAMP1 输出引脚为 PA2，后续由 ADC_CH3 采样
	GPIO_InitPeripheral(GPIOA, &GPIO_InitStructure); 		//初始化 PA2
}	

/**
  ******************************************************************************
  * @brief  opa1 配置
  * @param  None.
  * @retval None.
  ******************************************************************************/
static void bsp_opa1_config(void)
{
	OPAMP_InitType OPAMP_Initial;
	
    OPAMP_StructInit(&OPAMP_Initial); 											//使用驱动默认值初始化 OPAMP 配置结构体
    OPAMP_Initial.Mod            = OPAMP_CS_PGA_EN; 				//选择 PGA 可编程增益放大模式
    OPAMP_Initial.Gain           = OPAMP_CS_PGA_GAIN_32;	  //设置内部 PGA 增益为 32 倍
    OPAMP_Initial.HighVolRangeEn = ENABLE; 									//VDDA 为 3.3 V，选择高电压工作范围
    OPAMP_Initial.TimeAutoMuxEn  = DISABLE;								  //不使用定时器自动切换输入通道
    OPAMP_Init(OPAMP1, &OPAMP_Initial); 										//将配置参数写入 OPAMP1 控制寄存器
    OPAMP_Enable(OPAMP1, ENABLE); 													//使能 OPAMP1，开始放大 PA7 输入信号
#if 0	//运放校准测试
	bsp_delay_ms(100); //等待运放模拟电路稳定
	OPAMP_CalibrationEnable(OPAMP1, ENABLE); //进入运放失调校准模式
	bsp_delay_ms(100); //等待校准过程完成
	OPAMP_CalibrationEnable(OPAMP1, DISABLE); //退出校准模式，恢复正常工作
#endif	
}


/**
  ******************************************************************************
  * @brief  opa1 初始化
  * @param  None.
  * @retval None.
  ******************************************************************************/
void bsp_opa1_init(void)
{
	bsp_opa1_rcc_config(); 		//开启 OPAMP1 和 GPIOA 相关时钟
	bsp_opa1_io_config();   	//配置 PA7 运放输入和 PA2 运放输出
	bsp_opa1_config();			  //配置 PGA 参数并使能 OPAMP1
}
