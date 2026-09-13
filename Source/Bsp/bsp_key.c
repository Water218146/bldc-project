#include "bsp_key.h"

typedef struct
{
	key_num_e num;
	GPIO_Module* GPIOx;
	GPIO_InitType GPIO_InitStructure;
}gpio_config_t;


static gpio_config_t key_config[] = 
{
	START_STOP_KEY, GPIOC, {GPIO_PIN_6, GPIO_DC_2mA, GPIO_Slew_Rate_High, GPIO_Pull_Down, GPIO_Mode_Input, GPIO_NO_AF},
	CW_CCW_KEY    , GPIOC, {GPIO_PIN_7, GPIO_DC_2mA, GPIO_Slew_Rate_High, GPIO_Pull_Down, GPIO_Mode_Input, GPIO_NO_AF},
	KEY_MAX       , (GPIO_Module*)NULL , {(uint16_t)NULL, (GPIO_CurrentType)NULL, (GPIO_SpeedType)NULL, (GPIO_PuPdType)NULL, (GPIO_ModeType)NULL, (uint32_t)NULL}
};

/**
  ******************************************************************************
  * @brief  key时钟初始化
  * @param  None.
  * @retval None.
  ******************************************************************************/
static void bsp_key_rcc_config(void)
{
	uint8_t i = 0;
	KEY_RCC_ENABLE();	//使能GPIOC时钟
}

/**
  ******************************************************************************
  * @brief  key初始化
  * @param  None.
  * @retval None.
  ******************************************************************************/
void bsp_key_init(void)
{
	uint8_t i = 0;
	bsp_key_rcc_config();	//使能GPIOC时钟
	for(i = 0; i < (int)KEY_MAX; i++)
	{
		GPIO_InitPeripheral(key_config[i].GPIOx, &key_config[i].GPIO_InitStructure);
	}
}

/**
  ******************************************************************************
  * @brief  按键扫描
  * @param  None.
  * @retval None.
  ******************************************************************************/
static uint8_t key_scan(void) 
{
    uint8_t key_state = 0;
    if(GPIO_ReadInputDataBit(key_config[(int)CW_CCW_KEY].GPIOx, key_config[(int)CW_CCW_KEY].GPIO_InitStructure.Pin)==KEY_INPUT_POLARTY)
        key_state |= KEY_BIT(CW_CCW_KEY);
    if(GPIO_ReadInputDataBit(key_config[(int)START_STOP_KEY].GPIOx, key_config[(int)START_STOP_KEY].GPIO_InitStructure.Pin)==KEY_INPUT_POLARTY)
        key_state |= KEY_BIT(START_STOP_KEY);
    return key_state;
}

uint8_t key_val , key_down , key_up , key_old;

void key_task(void) {
    key_val = key_scan();
    key_down = key_val & (key_val ^ key_old);
    key_up = ~key_val & (key_val ^ key_old);
    key_old = key_val;
	
		if(key_down == KEY_BIT(CW_CCW_KEY))
		{
			ucled[0] = LED_ON;
			my_printf(DEBUG_COM,"cw_ccw_key:down!\r\n");
		}
		if(key_up == KEY_BIT(CW_CCW_KEY))
		{
			ucled[0] = LED_OFF;	
			my_printf(DEBUG_COM,"cw_ccw_key:up!\r\n");
		}
		if(key_down == KEY_BIT(START_STOP_KEY))
		{
			ucled[1] = LED_ON;
			my_printf(DEBUG_COM,"start_stop_key:down!\r\n");			
		}
		if(key_up == KEY_BIT(START_STOP_KEY))
		{
			ucled[1] = LED_OFF;		
			my_printf(DEBUG_COM,"start_stop_key:up!\r\n");	
		}
		
    
}