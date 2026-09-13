#include "bsp_led.h"

typedef struct
{
	led_num_e num;
	GPIO_Module* GPIOx;
	GPIO_InitType GPIO_InitStructure;
}gpio_config_t;

static gpio_config_t led_config[] = 
{
    
    LED1, GPIOC, {GPIO_PIN_13, GPIO_DC_2mA, GPIO_Slew_Rate_High, GPIO_Pull_Down, GPIO_Mode_Out_PP, GPIO_NO_AF},
    LED2, GPIOC, {GPIO_PIN_14, GPIO_DC_2mA, GPIO_Slew_Rate_High, GPIO_Pull_Down, GPIO_Mode_Out_PP, GPIO_NO_AF},
    LED3, GPIOC, {GPIO_PIN_15, GPIO_DC_2mA, GPIO_Slew_Rate_High, GPIO_Pull_Down, GPIO_Mode_Out_PP, GPIO_NO_AF},
    LED_MAX , (GPIO_Module*)NULL , {(uint16_t)NULL, (GPIO_CurrentType)NULL, (GPIO_SpeedType)NULL, (GPIO_PuPdType)NULL, (GPIO_ModeType)NULL, (uint32_t)NULL}
};

/**
  ******************************************************************************
  * @brief  led时钟初始化
  * @param  None.
  * @retval None.
  ******************************************************************************/
static void bsp_led_rcc_config(void)
{
    LED_RCC_ENABLE();	//使能GPIOC时钟
}   

/**
  ******************************************************************************
  * @brief  led初始化
  * @param  None.
  * @retval None.
  ******************************************************************************/
void bsp_led_init(void)
{
	uint8_t i = 0;
	bsp_led_rcc_config();	//使能GPIOC时钟
	for(i = 0; i < (int)LED_MAX; i++)
	{
		GPIO_InitPeripheral(led_config[i].GPIOx, &led_config[i].GPIO_InitStructure);
	}
}

uint8_t ucled[LED_MAX] = {LED_OFF,LED_OFF,LED_ON};

void led_disp(uint8_t *ucled)
{
    uint8_t i = 0;
    uint8_t temp = 0x00;
    static uint8_t temp_old = 0xff;
    for(i= 0;i<LED_MAX;i++)
    {
        temp |= (ucled[i]<<i);
    }
    if(temp != temp_old)
    {   
        GPIO_WriteBit(led_config[LED1].GPIOx, led_config[LED1].GPIO_InitStructure.Pin, (temp & 0x01)? LED_ON : LED_OFF);
        GPIO_WriteBit(led_config[LED2].GPIOx, led_config[LED2].GPIO_InitStructure.Pin, (temp & 0x02)? LED_ON : LED_OFF);
        GPIO_WriteBit(led_config[LED3].GPIOx, led_config[LED3].GPIO_InitStructure.Pin, (temp & 0x04)? LED_ON : LED_OFF);
        // Update the LED states
        temp_old = temp;
    }   
}

void led_task(void)
{
    
    led_disp(ucled);
}
