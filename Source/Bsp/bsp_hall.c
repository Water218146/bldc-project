#include "bsp_hall.h"

hall_irq_cb_t hall_irq_cb = {NULL};

/**
  ******************************************************************************
  * @brief  hall中断开启
  * @param  None.
  * @retval None.
  ******************************************************************************/
void bsp_hall_irq_enable(void)
{
	EXTI_InitType EXTI_InitStructure = {0};
	NVIC_InitType NVIC_InitStructure = {0};
	
	EXTI_InitStructure.EXTI_Line			= EXTI_LINE1;						//PC1 HALL_U
	EXTI_InitStructure.EXTI_Mode			= EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger 	= EXTI_Trigger_Rising_Falling;//io上升沿/下降沿触发
	EXTI_InitStructure.EXTI_LineCmd 	= ENABLE;
	EXTI_InitPeripheral(&EXTI_InitStructure);
	
	EXTI_InitStructure.EXTI_Line			= EXTI_LINE2;						//PC2 HALL_V
	EXTI_InitPeripheral(&EXTI_InitStructure);

	EXTI_InitStructure.EXTI_Line			= EXTI_LINE3;						//PC3 HALL_W
	EXTI_InitPeripheral(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel										=	EXTI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority	= 0;	//主优先级最高
	NVIC_InitStructure.NVIC_IRQChannelSubPriority					= 0;	//子优先级同样设置为最高
	NVIC_InitStructure.NVIC_IRQChannelCmd									= ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel										=	EXTI2_IRQn;
	NVIC_Init(&NVIC_InitStructure);	
	
	NVIC_InitStructure.NVIC_IRQChannel										=	EXTI3_IRQn;
	NVIC_Init(&NVIC_InitStructure);	
}

/**
  ******************************************************************************
  * @brief  hall中断关闭
  * @param  None.
  * @retval None.
  ******************************************************************************/
void bsp_hall_irq_disable(void)
{
	EXTI_InitType EXTI_InitStructure = {0};
	NVIC_InitType NVIC_InitStructure = {0};
	
	EXTI_InitStructure.EXTI_Line			= EXTI_LINE1;						//PC1 HALL_U
	EXTI_InitStructure.EXTI_Mode			= EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger 	= EXTI_Trigger_Rising_Falling;//io上升沿/下降沿触发
	EXTI_InitStructure.EXTI_LineCmd 	= DISABLE;
	EXTI_InitPeripheral(&EXTI_InitStructure);
	
	EXTI_InitStructure.EXTI_Line			= EXTI_LINE2;						//PC2 HALL_V
	EXTI_InitPeripheral(&EXTI_InitStructure);

	EXTI_InitStructure.EXTI_Line			= EXTI_LINE3;						//PC3 HALL_W
	EXTI_InitPeripheral(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel										=	EXTI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority	= 3;	//主优先级最高
	NVIC_InitStructure.NVIC_IRQChannelSubPriority					= 3;	//子优先级同样设置为最高
	NVIC_InitStructure.NVIC_IRQChannelCmd									= DISABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel										=	EXTI2_IRQn;
	NVIC_Init(&NVIC_InitStructure);	
	
	NVIC_InitStructure.NVIC_IRQChannel										=	EXTI3_IRQn;
	NVIC_Init(&NVIC_InitStructure);	
}

/**
  ******************************************************************************
  * @brief  hall初始化
  * @param  irq_cb:中断回调指针数组
  * @param  formal_param:中断回调指针数组中形参
  * @retval None.
  ******************************************************************************/
void bsp_hall_init(void(*irq_cb[3])(void(*formal_param)(void)),void (*formal_param)(void))
{
	GPIO_InitType GPIO_InitStructure = {0};
	if((irq_cb[0] == NULL)||(irq_cb[1] == NULL)||(irq_cb[2] == NULL))
	{
		while(1);
	}
	hall_irq_cb.hall_u_cb = irq_cb[0];	//回调赋值
	hall_irq_cb.hall_v_cb = irq_cb[1];	//回调赋值
	hall_irq_cb.hall_w_cb = irq_cb[2];	//回调赋值
	hall_irq_cb.formal_param = formal_param;
	
	//Enable GPIO clocks
  RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_AFIO | RCC_APB2_PERIPH_GPIOC, ENABLE);
	
	//IO Config HALL:U->PC1 HALL:V->PC2 HALL:W->PC3
	GPIO_InitStruct(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Current = GPIO_DC_4mA;
	GPIO_InitStructure.GPIO_Pull    = GPIO_No_Pull;
	GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_Input;
	GPIO_InitStructure.Pin					= GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3;
	GPIO_InitPeripheral(GPIOC,&GPIO_InitStructure);
	
	/*Configure io EXTI Line to io input Pin*/
	GPIO_ConfigEXTILine(GPIOC_PORT_SOURCE,GPIO_PIN_SOURCE1);
	GPIO_ConfigEXTILine(GPIOC_PORT_SOURCE,GPIO_PIN_SOURCE2);
	GPIO_ConfigEXTILine(GPIOC_PORT_SOURCE,GPIO_PIN_SOURCE3);

	bsp_hall_irq_disable();							//默认不开启hall的中断 防止电机在失能状态下被转动而导致进入了hall的cb函数
}

void hall_sensor_test(void)
{
	motor_get_hall_value();
	my_printf(DEBUG_COM,"hall_u:%d,hall_v:%d,hall_w:%d\r\n",hall_value.u_value,hall_value.v_value,hall_value.w_value);
}